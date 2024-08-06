import pyttsx3, api
import speech_recognition as sr

engine = pyttsx3.init()
recognizer = sr.Recognizer()
engine.setProperty('rate', 150)
engine.setProperty('volume', 1.0)

def speak(text):
    engine.say(text)
    engine.runAndWait()

def listen():
    with sr.Microphone() as source:
        recognizer.adjust_for_ambient_noise(source)
        audio = recognizer.listen(source)
        try:
            text = recognizer.recognize_whisper(audio, model="large", language="zh")
            return text
        except sr.UnknownValueError:
            print("无法理解")
        except sr.RequestError as e:
            print(f"无法请求语音识别服务；{e}")

def command_handler(command):
    if command == "开灯":
        api.send_message("{\"light\":1}")
    elif command == "关灯":
        api.send_message("{\"light\":0}")
    else:
        pass
        
if __name__ == "__main__":
    while True:
        command = listen()
        print("Text: " + command)
        command_handler(command)
