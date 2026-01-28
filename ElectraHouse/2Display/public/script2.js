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

    // Segment 2 logic (Room 2)
    if (msg === "Room 2: Card#: 23 47 89 0D") {
        changeVideo('segment2', 'videos/video3.mp4');
        console.log("Switched to video3.mp4 for segment2");
    } else if (msg === "Room 2: Card#: E3 66 B8 0D") {
        changeVideo('segment2', 'videos/video4.mp4');
        console.log("Switched to video4.mp4 for segment2");
    } else {
        console.log("Message not handled:", msg);
    }
});
