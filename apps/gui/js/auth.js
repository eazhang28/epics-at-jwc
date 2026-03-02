// Simple client-side auth for demo purposes only.
// Stores users in localStorage under 'jwc_users' and session in sessionStorage 'jwc_session'.

function getUsers(){
  try{
    return JSON.parse(localStorage.getItem('jwc_users')||'{}');
  }catch(e){return {}};
}

function saveUsers(users){
  localStorage.setItem('jwc_users', JSON.stringify(users));
}

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

  const users = getUsers();
  if(users[username]){ setMessage('User already exists. Please login.', true); return; }

  users[username] = { password };
  saveUsers(users);
  setMessage('Registered successfully — you can now login.');
}

function loginUser(){
  const username = (document.getElementById('username')?.value||'').trim();
  const password = (document.getElementById('password')?.value||'').trim();
  if(!username || !password){ setMessage('Username and password required', true); return; }

  const users = getUsers();
  const u = users[username];
  if(!u || u.password !== password){ setMessage('Invalid credentials', true); return; }

  // set session
  sessionStorage.setItem('jwc_session', JSON.stringify({ username, ts: Date.now() }));
  setMessage('Login successful — redirecting...');
  setTimeout(()=>{ location.href = 'index.html#write'; }, 700);
}

function logoutUser(){
  sessionStorage.removeItem('jwc_session');
  location.href = 'login.html';
}

// Auto-redirect logged-in users away from login page
if(location.pathname.endsWith('login.html')){
  try{
    if(sessionStorage.getItem('jwc_session')) location.href = 'index.html#write';
  }catch(e){}
}
