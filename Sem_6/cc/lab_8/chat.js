// chat.js

let currentRoomId = null;
let pollInterval = null;
let lastMessageCount = 0;
let currentUser = "";

// Join a chat room
function joinRoom() {
  const roomInput = document.getElementById("room-id").value.trim();
  if (!roomInput) {
    alert("Please enter a Room ID to join.");
    return;
  }

  currentRoomId = roomInput;
  document.getElementById("room-label").innerText = currentRoomId;
  document.getElementById("header-room-id").innerText = currentRoomId;
  document.getElementById("room-indicator").style.display = "inline-flex";
  document.getElementById("chat-section").style.display = "flex";
  document.getElementById("join-section").style.display = "none";

  if (pollInterval) clearInterval(pollInterval);
  loadMessages();
  pollInterval = setInterval(loadMessages, CONFIG.pollInterval);
  
  // Save room and sender in localStorage
  saveToStorage(CONFIG.storage.roomKey, currentRoomId);
  
  // Focus message input
  setTimeout(() => {
    document.getElementById("message").focus();
  }, 300);
}

// Leave the current chat room
function leaveRoom() {
  if (pollInterval) clearInterval(pollInterval);
  currentRoomId = null;
  lastMessageCount = 0;
  document.getElementById("room-indicator").style.display = "none";
  document.getElementById("chat-section").style.display = "none";
  document.getElementById("join-section").style.display = "block";
  document.getElementById("chat-box").innerHTML = "";
}

// Send a message to the current room
function sendMessage() {
  const sender = document.getElementById("sender").value.trim();
  const message = document.getElementById("message").value.trim();

  if (!sender) {
    alert("Please enter your name.");
    document.getElementById("sender").focus();
    return;
  }
  
  if (!message) {
    document.getElementById("message").focus();
    return;
  }
  
  if (!currentRoomId) {
    alert("Please join a room first.");
    return;
  }
  
  currentUser = sender;
  saveToStorage(CONFIG.storage.senderKey, sender);

  // Show sending indicator immediately
  const tempId = 'msg-' + Date.now();
  addMessageToUI({
    Sender: sender,
    Message: message,
    tempId: tempId,
    sending: true
  });

  fetch(CONFIG.apiEndpoints.insertUrl, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ RoomId: currentRoomId, Sender: sender, Message: message })
  })
  .then(response => response.json())
  .then(data => {
    console.log("Message sent:", data);
    document.getElementById("message").value = "";
    // Replace the temporary message indicator
    const tempMsg = document.getElementById(tempId);
    if (tempMsg) {
      tempMsg.querySelector('.message-content').classList.remove('sending');
    }
    loadMessages();
  })
  .catch(err => {
    console.error("Send error:", err);
    // Show error state
    const tempMsg = document.getElementById(tempId);
    if (tempMsg) {
      tempMsg.querySelector('.message-content').classList.add('error');
    }
  });
}

// Add a message to the UI
function addMessageToUI(msg) {
  const chatBox = document.getElementById("chat-box");
  const isMyMessage = msg.Sender === currentUser;
  const initials = getInitials(msg.Sender);
  const avatarColor = stringToColor(msg.Sender);
  
  const messageElement = document.createElement('div');
  messageElement.className = `message ${isMyMessage ? 'my-message' : ''}`;
  if (msg.tempId) {
    messageElement.id = msg.tempId;
  }
  
  messageElement.innerHTML = `
    <div class="message-avatar" style="background-color: ${avatarColor}">
      ${initials}
    </div>
    <div class="message-content ${msg.sending ? 'sending' : ''}">
      <div class="message-sender">${msg.Sender}</div>
      <div class="message-text">${msg.Message}</div>
      <div class="message-time">${formatTimestamp()}</div>
    </div>
  `;
  
  chatBox.appendChild(messageElement);
  chatBox.scrollTop = chatBox.scrollHeight;
}

// Load messages from the server
function loadMessages() {
  if (!currentRoomId) return;

  fetch(CONFIG.apiEndpoints.baseGetUrl + encodeURIComponent(currentRoomId))
    .then(response => response.json())
    .then(messages => {
      if (messages.length === lastMessageCount) {
        return; // No new messages
      }
      
      const chatBox = document.getElementById("chat-box");
      chatBox.innerHTML = "";
      
      messages.forEach(msg => {
        addMessageToUI(msg);
      });
      
      // If there are new messages and not the first load
      if (lastMessageCount > 0 && messages.length > lastMessageCount) {
        playNotificationSound();
      }
      
      lastMessageCount = messages.length;
    })
    .catch(err => console.error("Load error:", err));
}

// Initialize the application
window.onload = function() {
  const savedRoom = getFromStorage(CONFIG.storage.roomKey);
  const savedSender = getFromStorage(CONFIG.storage.senderKey);
  
  if (savedSender) {
    document.getElementById("sender").value = savedSender;
    currentUser = savedSender;
  }
  
  if (savedRoom) {
    document.getElementById("room-id").value = savedRoom;
    // Auto-join if there's a saved room (uncomment to enable)
    // joinRoom();
  }
};