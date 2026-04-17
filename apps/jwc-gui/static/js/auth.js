// Simple client-side auth for demo purposes only.
// Stores users in localStorage under 'jwc_users' and session in sessionStorage 'jwc_session'.

function setMessage(msg, isError){
  const el = document.getElementById('message');
  if(!el) return;
  el.innerText = msg;
  el.style.color = isError ? 'crimson' : 'green';
}

function registerUser(){
  const username = (document.getElementById('username')?.value||'').trim();
  const password = (document.getElementById('password')?.value||'').trim();
  if(!username || !password){ setMessage('Username and password required', true); return; }

  fetch('/api/register', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ username, password })
  })
  .then(res => res.json())
  .then(data => {
    if (data.error) setMessage(data.error, true);
    else setMessage('Registered successfully — you can now login.');
  })
  .catch(err => setMessage('Error registering', true));
}

function loginUser(){
  const username = (document.getElementById('username')?.value||'').trim();
  const password = (document.getElementById('password')?.value||'').trim();
  if(!username || !password){ setMessage('Username and password required', true); return; }

  fetch('/api/login', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ username, password })
  })
  .then(res => res.json())
  .then(data => {
    if (data.error) setMessage(data.error, true);
    else {
      sessionStorage.setItem('jwc_session', JSON.stringify({ username, ts: Date.now() }));
      setMessage('Login successful — redirecting...');
      setTimeout(()=>{ location.href = '/write'; }, 700);
    }
  })
  .catch(err => setMessage('Error logging in', true));
}

function logoutUser(){
  sessionStorage.removeItem('jwc_session');
  location.href = '/login';
}

// Auto-redirect logged-in users away from login page
if(location.pathname === '/login' || location.pathname.endsWith('login.html')){
  try{
    if(sessionStorage.getItem('jwc_session')) location.href = '/write';
  }catch(e){}
}
