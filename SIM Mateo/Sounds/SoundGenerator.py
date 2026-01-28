import numpy as np
import wave

def generate_square_wave(filename, frequency=440, duration=1.0, sample_rate=44100):
    """
    Generate an 8-bit square wave sound and save it as a WAV file.
    
    Args:
        filename (str): Name of the output WAV file.
        frequency (float): Frequency of the tone in Hertz.
        duration (float): Duration of the tone in seconds.
        sample_rate (int): Samples per second.
    """
    # 8-bit audio has a range of 0 to 255. We'll center the wave at 128.
    amplitude = 127
    n_samples = int(sample_rate * duration)
    
    # Generate square wave: if sin(2*pi*f*t) >= 0, use +amplitude; else, use -amplitude.
    # Then offset to unsigned 8-bit by adding 128.
    t = np.linspace(0, duration, n_samples, endpoint=False)
    wave_data = np.where(np.sin(2 * np.pi * frequency * t) >= 0, amplitude, -amplitude)
    wave_data = (wave_data + 128).astype(np.uint8)  # Convert to unsigned 8-bit integers
    
    # Write the wave data to a file
    with wave.open(filename, 'wb') as wav_file:
        wav_file.setnchannels(1)      # Mono
        wav_file.setsampwidth(1)      # 8-bit audio
        wav_file.setframerate(sample_rate)
        wav_file.writeframes(wave_data.tobytes())

# Example usage: generate a 440 Hz tone lasting 1 second.
generate_square_wave("square_wave.wav", frequency=440, duration=1.0)
