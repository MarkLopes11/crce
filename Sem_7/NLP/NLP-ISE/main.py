import re
import random
import time
from collections import deque
from textblob import TextBlob
import nltk
from nltk.tokenize import word_tokenize
from nltk.corpus import stopwords
import json
import logging
import nltk
nltk.download('punkt', download_dir=r'C:\Users\Mark Lopes\nltk_data')
nltk.download('punkt_tab', download_dir=r'C:\Users\Mark Lopes\nltk_data')

# Download required NLTK data (show errors if any)

nltk.download('stopwords', download_dir=r'C:\Users\Mark Lopes\nltk_data')
nltk.download('averaged_perceptron_tagger', download_dir=r'C:\Users\Mark Lopes\nltk_data')

class EnhancedChatbot:
    def __init__(self):
        self.conversation_history = deque(maxlen=5)  # Remember last 5 conversations
        self.user_name = None
        self.mood = 0  # Bot's mood (-1 to 1)
        
        # Load response patterns from JSON
        self.response_patterns = {
            "greetings": {
                "patterns": r'hi|hello|hey|good\s*(morning|evening|afternoon)',
                "responses": {
                    "positive": ["Hello! Great to see you!", "Hi there! How's your day going?"],
                    "neutral": ["Hello!", "Hi there!"],
                    "negative": ["Hey...", "Hi."]
                }
            },
            "farewell": {
                "patterns": r'bye|goodbye|see\s*you|exit',
                "responses": {
                    "positive": ["Goodbye! It was wonderful chatting!", "See you soon!"],
                    "neutral": ["Goodbye!", "See you later!"],
                    "negative": ["Bye then...", "Ok, goodbye."]
                }
            },
            # Add more complex patterns here
        }
        
        self.logger = self.setup_logger()

    def setup_logger(self):
        logger = logging.getLogger('ChatbotLogger')
        logger.setLevel(logging.INFO)
        # Prevent adding multiple handlers if re-instantiated
        if not logger.handlers:
            handler = logging.FileHandler('chatbot_logs.log')
            formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
            handler.setFormatter(formatter)
            logger.addHandler(handler)
        return logger

    def analyze_sentiment(self, text):
        analysis = TextBlob(text)
        return analysis.sentiment.polarity

    def preprocess_input(self, user_input):
        # Tokenize and remove stopwords
        tokens = word_tokenize(user_input.lower())
        stop_words = set(stopwords.words('english'))
        return [token for token in tokens if token not in stop_words]

    def update_mood(self, user_sentiment):
        self.mood = max(min(self.mood + user_sentiment * 0.5, 1), -1)

    def get_response_type(self):
        if self.mood > 0.3:
            return "positive"
        elif self.mood < -0.3:
            return "negative"
        return "neutral"

    def generate_response(self, user_input, processed_tokens):
        # Log input
        self.logger.info(f"User input: {user_input}")
        
        # Analyze sentiment
        sentiment = self.analyze_sentiment(user_input)
        self.update_mood(sentiment)
        
        # Store in conversation history
        self.conversation_history.append((user_input, sentiment))
        
        # Check for name if not known
        if not self.user_name and re.search(r'my name is (\w+)', user_input.lower()):
            self.user_name = re.search(r'my name is (\w+)', user_input.lower()).group(1)
            return f"Nice to meet you, {self.user_name.capitalize()}!"

        # Get response based on patterns
        for intent, data in self.response_patterns.items():
            if re.search(data["patterns"], user_input.lower()):
                response_type = self.get_response_type()
                response = random.choice(data["responses"][response_type])
                
                # Add name if known
                if self.user_name and random.random() < 0.3:
                    response = f"{self.user_name.capitalize()}, {response}"
                
                return response

        # Default response with context
        return self.generate_default_response(processed_tokens)

    def generate_default_response(self, processed_tokens):
        if not processed_tokens:
            return "I'm not sure I understand. Could you rephrase that?"
        
        # Generate response based on POS tagging
        pos_tags = nltk.pos_tag(processed_tokens)
        
        # If question is detected
        if any(tag in ['WP', 'WRB', 'WDT'] for word, tag in pos_tags):
            return "That's an interesting question. Could you elaborate?"
        
        # If command is detected
        if any(tag == 'VB' for word, tag in pos_tags):
            return "I'm not sure I can do that, but I'm learning!"
        
        return "I understand the words, but I'm not sure how to respond appropriately."

def main():
    chatbot = EnhancedChatbot()
    print("Enhanced ChatBot: Hello! Type 'bye' to exit.")
    
    while True:
        try:
            user_input = input("You: ").strip()
            if not user_input:
                continue
                
            if user_input.lower() == 'bye':
                print("ChatBot:", random.choice(chatbot.response_patterns["farewell"]["responses"]["neutral"]))
                break
                
            processed_tokens = chatbot.preprocess_input(user_input)
            response = chatbot.generate_response(user_input, processed_tokens)
            
            # Simulate thinking
            time.sleep(random.uniform(0.5, 1.5))
            print("ChatBot:", response)
            
        except Exception as e:
            chatbot.logger.error(f"Error occurred: {str(e)}")
            print(f"ChatBot: Sorry, I encountered an error: {e}")
            print("ChatBot: Sorry, I encountered an error. Let's continue our conversation.")

if __name__ == "__main__":
    main()