from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, ContextTypes, CommandHandler, MessageHandler, filters
import os
YOUR_BOT_TOKEN = '' '
user_message = []
user_id =[]
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

# Message handler
async def handle_message(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text("only start, info and help are supported. ")
    await update.message.reply_text(update.message.from_user.username)
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