import pyttsx3

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
text = "Hello! lets me know this can help you . hope you are doing well."
text_to_speech(text, output_file="output_audio.mp3")  # Save to file
# text_to_speech(text)  # Speak directly
