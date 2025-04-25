// config.js
// API Endpoints & Configuration
const CONFIG = {
    apiEndpoints: {
      insertUrl: "https://g7cnmc9bpf.execute-api.ap-south-1.amazonaws.com/InsertChatMessage",
      baseGetUrl: "https://g7cnmc9bpf.execute-api.ap-south-1.amazonaws.com/GetChatMessages?RoomId="
    },
    pollInterval: 2000,
    storage: {
      roomKey: "chatRoom",
      senderKey: "chatSender"
    },
    avatarColors: [
      '#4f46e5', '#10b981', '#f59e0b', '#ef4444', 
      '#8b5cf6', '#ec4899', '#06b6d4', '#f97316'
    ],
    notificationSound: "notification.mp3"
  };