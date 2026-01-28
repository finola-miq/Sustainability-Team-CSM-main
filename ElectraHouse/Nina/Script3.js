let state = 0;
const videos = document.querySelectorAll("video");

function changeVideo(segmentId, newVideoSrc) {
    const segment = document.getElementById(segmentId);
    const video = segment.querySelector("video");
    video.src = newVideoSrc;
    video.play();
}

const socket = new WebSocket("ws://localhost:80");

socket.addEventListener("open", () => {
    console.log("WebSocket connected");
});

socket.addEventListener("message", (event) => {
    const msg = event.data.replace(/\s/g, "").toLowerCase();

    // When "start" is received, play video1.mp4 in segment1
    if (msg === "Reader 1: Card UID: 73 82 DB 0D") {
        console.log("Start command received - playing video1.mp4 in segment1");
        changeVideo("segment1", "videos/video1.mp4");
        state = 0; // Adjust state if needed
        return;
    }

    // Pause any currently playing videos for immediate switching
    videos.forEach((video) => {
        video.pause();
    });

    // Change videos based on numeric messages
    if (msg === "Reader 1: Card UID: 73 98 80 13") {
        console.log("Message 1 received - playing corresponding video");
        changeVideo("segment1", "videos/video2.mp4");
        state = 1;
    } else if (msg === "Reader 0: Card UID: 23 47 89 0D") {
        console.log("Message 2 received - playing corresponding video");
        changeVideo("segment3", "videos/video6.mp4");
        state = 2;
    } else if (msg === "Reader 0: Card UID: E3 66 B8 0D") {
        console.log("Message 3 received - playing corresponding videos");
        // For message 3, update two segments simultaneously
        changeVideo("segment4", "videos/video8.mp4");
        changeVideo("segment2", "videos/video4.mp4");
        state = 3;
    } else {
        console.log("Unknown message:", msg);
    }
});
