import numpy as np
import wave

def generate_square_wave(frequency, duration, sample_rate=44100):
    """
    Generate an 8-bit square wave for a given frequency and duration.
    
    Args:
        frequency (float): Frequency of the note in Hertz.
        duration (float): Duration of the note in seconds.
        sample_rate (int): Number of samples per second.
        
    Returns:
        numpy.ndarray: Array of unsigned 8-bit integers representing the waveform.
    """
    amplitude = 127  # 8-bit audio amplitude range for a centered square wave
    n_samples = int(sample_rate * duration)
    t = np.linspace(0, duration, n_samples, endpoint=False)
    # Create a square wave: use +amplitude when sin >= 0; else -amplitude.
    wave_data = np.where(np.sin(2 * np.pi * frequency * t) >= 0, amplitude, -amplitude)
    # Offset to unsigned 8-bit range (0-255)
    return (wave_data + 128).astype(np.uint8)

def generate_win_sound(filename, sample_rate=44100):
    """
    Generate a celebratory win sound effect by combining several notes.
    
    The sound effect consists of a series of three notes with a brief pause between each.
    
    Args:
        filename (str): The output WAV file name.
        sample_rate (int): Samples per second.
    """
    # Define a sequence of notes (frequency in Hz, duration in seconds)
    # Here we choose a pleasant, ascending chord: C5, E5, and G5.
    notes = [
        (523, 0.2),  # C5 for 0.2 seconds
        (659, 0.2),  # E5 for 0.2 seconds
        (784, 0.2)   # G5 for 0.2 seconds
    ]
    
    # Optional: Add a short pause (silence) between notes (50ms).
    pause_duration = 0.05  
    pause_samples = np.full(int(sample_rate * pause_duration), 128, dtype=np.uint8)
    
    # Combine all notes and pauses into one sound sequence.
    sound_data = np.array([], dtype=np.uint8)
    for frequency, duration in notes:
        tone = generate_square_wave(frequency, duration, sample_rate)
        sound_data = np.concatenate((sound_data, tone, pause_samples))
    
    # Write the combined sound data to a WAV file.
    with wave.open(filename, 'wb') as wav_file:
        wav_file.setnchannels(1)      # Mono sound
        wav_file.setsampwidth(1)      # 8-bit audio
        wav_file.setframerate(sample_rate)
        wav_file.writeframes(sound_data.tobytes())

# Example usage: generate the win sound effect and save it as "win_sound.wav".
generate_win_sound("win_sound.wav")
