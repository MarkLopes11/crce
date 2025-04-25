// utils.js

// Generate a random color for user avatars
function stringToColor(str) {
    let hash = 0;
    for (let i = 0; i < str.length; i++) {
      hash = str.charCodeAt(i) + ((hash << 5) - hash);
    }
    return CONFIG.avatarColors[Math.abs(hash) % CONFIG.avatarColors.length];
  }
  
  // Get initials from name
  function getInitials(name) {
    return name.split(' ')
      .map(word => word.charAt(0))
      .join('')
      .toUpperCase()
      .substring(0, 2);
  }
  
  // Format timestamp
  function formatTimestamp() {
    const now = new Date();
    return now.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  }
  
  // Play notification sound
  function playNotificationSound() {
    const audio = new Audio(CONFIG.notificationSound);
    audio.play();
  }
  
  // Handle key press events
  function handleKeyPress(event) {
    if (event.key === 'Enter') {
      sendMessage();
    }
  }
  
  // Toggle emoji picker (placeholder)
  function toggleEmojiPicker() {
    alert("Emoji picker feature coming soon!");
  }
  
  // Save to local storage
  function saveToStorage(key, value) {
    localStorage.setItem(key, value);
  }
  
  // Get from local storage
  function getFromStorage(key) {
    return localStorage.getItem(key);
  }