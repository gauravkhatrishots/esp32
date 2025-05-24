import requests

BOT_TOKEN = 'your_bot_token_here'
CHAT_ID = 'your_chat_id_here'
MESSAGE = 'Hello from Python! 🐍'

url = f'https://api.telegram.org/bot{BOT_TOKEN}/sendMessage'

payload = {
    'chat_id': CHAT_ID,
    'text': MESSAGE
}

response = requests.post(url, data=payload)

# Optional: check the response
if response.status_code == 200:
    print("Message sent successfully.")
else:
    print("Failed to send message:", response.text)
