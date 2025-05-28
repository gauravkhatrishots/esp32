from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, ContextTypes, CommandHandler, MessageHandler, filters
import os
import pyttsx3
from  gtts import gTTS


def text_to_speech(text, output_file=None):
    engine = pyttsx3.init()

    # Optional: Set voice (male/female), rate, and volume
    engine.setProperty('rate', 150)     # Speed (default 200)
    engine.setProperty('volume', 1.0)   # Volume (0.0 to 1.0)

    voices = engine.getProperty('voices')
    engine.setProperty('voice', voices[0].id)  # voices[0] = male, voices[1] = female (may vary by OS)

    if output_file:
        engine.save_to_file(text, output_file)
        engine.runAndWait()
        print(f"Audio saved to {output_file}")
    else:
        engine.say(text)
        engine.runAndWait()

# Example usage:
# text = "Hello! lets me know this can help you . hope you are doing well."
# text_to_speech(text, output_file="output_audio.mp3")  # Save to file
# text_to_speech(text)  # Speak directly
YOUR_BOT_TOKEN = '8141510479:AAE-bvquH4CMHQJ_CrNqwIE7J74NnWp6Ki4'
user_message = []
user_id =[]

AUDIO_FILE_PATH = "output_audio.mp3"
# /start command handler
async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text("👋 Hello!  kya haal ,hows it💡 its a dummy interface , abhi deploy ni hua hai yaar to keep you happy its 18 degree here in montlucon")

# /help command handler
async def help_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text("💡 its a dummy interface , abhi deploy ni hua hai yaar")

async def send_image(update: Update, context: ContextTypes.DEFAULT_TYPE):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    image_path = os.path.join(script_dir, "imag_demo.jpg")
    with open(image_path, "rb") as image_file:
        await context.bot.send_photo(chat_id=update.effective_chat.id, photo=image_file)
        await update.message.reply_text("💡 not fully deployed,come after some more time")
    
async def send_image_live(update: Update, context: ContextTypes.DEFAULT_TYPE):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    image_path = os.path.join(script_dir, "imag_demo.jpg")
    with open(image_path, "rb") as image_file:
        await context.bot.send_photo(chat_id=update.effective_chat.id, photo=image_file)
        await update.message.reply_text("💡 thats the status bro, wait for next sprint")

async def send_audio(update: Update, context: ContextTypes.DEFAULT_TYPE):
    with open(AUDIO_FILE_PATH, 'rb') as audio:
        await update.message.reply_audio(audio=audio, caption="Here is your audio 🎵")

# Message handler
async def handle_message(update: Update, context: ContextTypes.DEFAULT_TYPE):
    print(update._effective_user.username)
    # text_to_speech("hi" +  str(update.effective_user.username) +" hows it going there", AUDIO_FILE_PATH )
    tts = gTTS(text="hi" +  str(update.effective_user.username) +" hows it going there", lang='en')
    tts.save(AUDIO_FILE_PATH)
    await update.message.reply_text("only start, info and help are supported. ") 
    await context.bot.send_audio(chat_id=update.effective_chat.id, audio=open(AUDIO_FILE_PATH, 'rb'), caption="This is a dummy audio response. Please use /start or /help for more information.")
    # Create and run the bot
if __name__ == '__main__':
    app = ApplicationBuilder().token(YOUR_BOT_TOKEN).build()

    # Add handlers for commands
    app.add_handler(CommandHandler("start", start))
    app.add_handler(CommandHandler("info", send_image_live))
    app.add_handler(CommandHandler('help', send_image))
    app.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, handle_message))
    print("🤖 Bot is running...")
    app.run_polling()


# 14 july to 18 
# 22 morning  3:30 pm 
# 8 days 
# helsinki 
 
#8141510479:AAE-bvquH4CMHQJ_CrNqwIE7J74NnWp6Ki4