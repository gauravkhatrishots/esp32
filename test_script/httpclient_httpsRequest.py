import requests
import os
# Your Telegram Bot Token here
BOT_TOKEN = "YOUR_BOT_TOKEN"

# Telegram getUpdates URL
url = f"https://api.telegram.org/bot8141510479:AAE-bvquH4CMHQJ_CrNqwIE7J74NnWp6Ki4/getUpdates"

# Path to your downloaded root CA PEM file
cert_path = os.path.join(os.path.dirname(__file__), "telegram.pem")
print("CA_CERT_PATH:", cert_path)

try:
    # Send HTTPS GET request using explicit CA cert for verification
    response = requests.get(url, verify=cert_path)

    # Raise exception if status is not 200 OK
    response.raise_for_status()

    # Print the JSON response from Telegram API
    print("Response JSON:")
    print(response.json())

except requests.exceptions.SSLError as ssl_err:
    print("SSL Certificate verification failed:", ssl_err)

except Exception as e:
    print("Request failed:", e)
