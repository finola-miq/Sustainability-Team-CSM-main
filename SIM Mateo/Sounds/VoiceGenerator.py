from gtts import gTTS
import os

def generate_voice_from_text(text, filename="output.mp3", lang="en"):
    """
    Generate an AI voice output from the provided text and save it as an MP3 file.

    Args:
        text (str): The text to convert to speech.
        filename (str): The output file name for the MP3.
        lang (str): The language code (default is English).
    """
    # Create the gTTS object
    tts = gTTS(text=text, lang=lang, slow=False)
    # Save the speech to a file
    tts.save(filename)
    print(f"Voice output saved as {filename}")

if __name__ == '__main__':
    user_text = input("Enter the text you want to convert to speech: ")
    generate_voice_from_text(user_text)
