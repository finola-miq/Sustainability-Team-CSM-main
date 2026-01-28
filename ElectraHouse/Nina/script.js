let state = 0;

function changeVideo(segmentId, newVideoSrc) {
  const segment = document.getElementById(segmentId);
  const video = segment.querySelector("video");
  video.src = newVideoSrc;
  video.play();
}

const socket = new WebSocket("ws://localhost:80");

socket.addEventListener("open", function (event) {
  console.log("WebSocket connected");
});

const endable = [false, false, false, false];
const video1 = document.querySelector("#video1");
/*
socket.addEventListener("message", function (event) {
  const msg = event.data.replaceAll(/\s/g, "").toString().toLowerCase();
  if (msg === "start" && !state) {
    video1.play();
    state++;
    return;
  }

  if (parseInt(msg) * 2 - 1 === state && state % 2 === 1) {
    console.log(state);
    changeVideo(`segment${(state + 1) / 2}`, `videos/video${state + 1}.mp4`);
    videos[(state - 1) / 2].addEventListener("ended", () => {
      videos[state / 2].play();
      state++;
    });
    state++;
  }
});
*/

socket.addEventListener("message", function (event) {
  const msg = event.data.replaceAll(/\s/g, "").toString().toLowerCase();
  if (msg === "start") {
    window.location.reload();
  }
  if (parseInt(msg) == 1 && state == 0) {
    console.log("1");
    changeVideo(`segment1`, `videos/video2.mp4`);
    videos[0].addEventListener("ended", () => {
      videos[2].play();
      state++;
    });
  }
  if (parseInt(msg) == 2 && state == 1) {
    console.log("2");
    changeVideo(`segment3`, `videos/video6.mp4`);
    videos[2].addEventListener("ended", () => {
      videos[3].play();
      state++;
    });
  }
  if (parseInt(msg) == 3 && state == 2) {
    console.log("3");
    changeVideo(`segment4`, `videos/video8.mp4`);
    changeVideo(`segment2`, `videos/video4.mp4`);
  }
});

const videos = document.querySelectorAll("video");
console.log(videos);
