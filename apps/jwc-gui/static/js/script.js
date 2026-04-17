/*let currentMessage = "";
let port;
let writer;

function showPage(pageId) {
    // Protect the write page: require a session
    if (pageId === 'write') {
    try {
        if (!sessionStorage.getItem('jwc_session')) {
        location.href = 'login.html';
        return;
        }
    } catch (e) {
        location.href = 'login.html';
        return;
    }
    }
    document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
    document.getElementById(pageId).classList.add('active');

    // update step indicator colors (only inside active page)
    document.querySelectorAll('.step').forEach(step => step.classList.remove('active'));

    const activePage = document.getElementById(pageId);
    const pageSteps = activePage.querySelectorAll('.step');

    if (pageId === 'write' && pageSteps[0]) {
    pageSteps[0].classList.add('active');
    }

    if (pageId === 'confirm' && pageSteps[1]) {
    pageSteps[1].classList.add('active');
    }

    if (pageId === 'confirm') {
    document.querySelectorAll('.step')[1]?.classList.add('active');
    }
}

// If user opens index.html with a hash (e.g. #write), navigate there.
document.addEventListener('DOMContentLoaded', () => {
    // Toggle auth link to Login / Logout depending on session
    const authLink = document.getElementById('authLink');
    if (authLink) {
    try {
        if (sessionStorage.getItem('jwc_session')) {
        authLink.innerText = 'Logout';
        authLink.href = '#';
        authLink.onclick = (e) => { e.preventDefault(); sessionStorage.removeItem('jwc_session'); location.reload(); };
        } else {
        authLink.innerText = 'Login';
        authLink.href = 'login.html';
        authLink.onclick = null;
        }
    } catch (e) {
        // ignore
    }
    }

    if (location.hash) {
    const id = location.hash.replace('#','');
    if (id === 'write') {
        // showPage will redirect to login if not authenticated
        window.location='write.html'
    } else if (document.getElementById(id)) {
        showPage(id);
    }
    }
});

function goToConfirm() {
    const text = document.getElementById('messageInput').value;
    currentMessage = text;
    document.getElementById('previewBox').innerText = text;
    showPage('confirm');
}

// ========== WEB SERIAL (BROWSER → ARDUINO) ==========

async function connectArduino() {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 9600 });
    writer = port.writable.getWriter();
}

async function sendToArduino() {
    try {
    if (!port) {
        await connectArduino();
    }

    const data = currentMessage + "\n";
    const encoder = new TextEncoder();
    await writer.write(encoder.encode(data));

    document.getElementById('status').innerText = "Message sent to drawbot successfully.";

    } catch (err) {
    document.getElementById('status').innerText = "Connection failed. Make sure Arduino is plugged in and try again.";
    }
}*/
// js/script.js

let currentMessage = "";
let port;
let writer;

// Toggle Login/Logout in nav
document.addEventListener('DOMContentLoaded', () => {
  const authLink = document.getElementById('authLink');
  if (authLink) {
    try {
      if (sessionStorage.getItem('jwc_session')) {
        authLink.innerText = 'Logout';
        authLink.href = '#';
        authLink.onclick = (e) => {
          e.preventDefault();
          sessionStorage.removeItem('jwc_session');
          location.href = '/';
        };
      } else {
        authLink.innerText = 'Login';
        authLink.href = '/login';
      }
    } catch (e) {}
  }
});

// ========== BACKEND SERIAL ==========

async function sendToArduino() {
  try {
    const res = await fetch('/send_cmd', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ input: currentMessage })
    });
    if (!res.ok) throw new Error('Send failed');
    
    sessionStorage.removeItem('jwc_pending_message'); // clean up after send
    document.getElementById('status').innerText = "Message sent to drawbot successfully.";
  } catch (err) {
    document.getElementById('status').innerText = "Connection failed. Please try again.";
  }
}

function goToConfirm() {
  const input = document.getElementById('messageInput');
  if (!input) return;
  const text = input.value.trim();
  if (!text) return;
  sessionStorage.setItem('jwc_pending_message', text);
  window.location.href = '/confirm';
}