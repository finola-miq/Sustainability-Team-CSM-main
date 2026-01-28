const startBtn = document.getElementById('startBtn');
const container = document.querySelector('.flex-container');
const videos = document.querySelectorAll('video');

startBtn.addEventListener('click', () => {
    startBtn.style.display = 'none';
    container.style.display = '';
    videos.forEach(video => {
        video.muted = false; // Enable sound
    });
});

function changeVideo(segmentId, newVideoSrc) {
    const segment = document.getElementById(segmentId);
    const video = segment.querySelector("video");
    video.src = newVideoSrc;
    video.play();
}

const socket = new WebSocket("ws://" + window.location.host);

socket.addEventListener("open", function(event) {
    console.log("WebSocket connected");
});

socket.addEventListener("message", function(event) {
    const msg = event.data.trim();
    console.log("Received message:", msg);

    // Segment 1 logic (Room 1)
    if (msg === "Room 1: Card#: 73 98 80 13") {
        changeVideo('segment1', 'videos/video2.mp4');
        console.log("Switched to video2.mp4 for segment1");
    } else if (msg === "Room 1: Card#: 73 82 DB 0D") {
        changeVideo('segment1', 'videos/video1.mp4');
        console.log("Switched to video1.mp4 for segment1");
    }

    // Segment 3 logic (Room 0)
    else if (msg === "Room 0: Card#: 93 90 C3 0D") {
        changeVideo('segment3', 'videos/video5.mp4');
        console.log("Switched to video5.mp4 for segment3");
    } else if (msg === "Room 0: Card#: 93 7C A9 0D") {
        changeVideo('segment3', 'videos/video6.mp4');
        console.log("Switched to video6.mp4 for segment3");
    } 

    else {
        console.log("Message not handled:", msg);
    }
});
