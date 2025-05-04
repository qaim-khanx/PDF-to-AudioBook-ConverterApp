# 🎧 PDF to Audio Converter (C++ Qt + Flask)

This is a simple and elegant desktop application that allows you to convert PDF documents into spoken audio using Google Text-to-Speech. It is built using **C++ and Qt for the frontend** and **Python with Flask** for the backend.

The app is ideal for educational use, especially to demonstrate:
- How to integrate a C++ GUI with a Python server
- How to send files via HTTP
- Basic text-to-speech capabilities

---

## 🌟 Features

✅ Select any PDF file from your system  
✅ Send PDF to a local Flask server for processing  
✅ Extract text using PyMuPDF and convert it to MP3 with gTTS  
✅ Download and automatically play the MP3 in the app  
✅ Only shows the **Convert** button after a file is selected  
✅ Clean and attractive UI with large fonts and color styling

---

## 🖥️ Technologies Used

### Frontend (Qt Desktop App)
- C++17 or later
- Qt 6 (Widgets, Network, Multimedia)

### Backend (Python Flask Server)
- Flask
- PyMuPDF (fitz)
- gTTS

---

## 📁 Project Structure

pdf-to-audio-converter/
│
├── main.cpp # Qt desktop application
├── PdfServer.py # Flask backend server
├── downloaded_audio.mp3 # (auto-generated when you run app)
├── README.md # This file

---

## 🔧 Setup Instructions

### ✅ 1. Backend Setup (Python)

Make sure Python 3.7+ is installed.

Install dependencies:
pip install flask gtts PyMuPDF
Run the server:

python PdfServer.py

You should see:
Running on http://127.0.0.1:5000

### 🧠 How It Works:

- PDF Selection: You choose a PDF file using a file dialog.

- HTTP Upload: The app sends it to the Python server using QHttpMultiPart.

- Text Extraction & TTS: Server extracts the text using PyMuPDF and converts it using gTTS.

- Return MP3: The server sends back the MP3 filename.

- Download & Play: Qt app downloads the file and plays it using QMediaPlayer.



### 📄 License
This project is open-source under the MIT License.

### 🙌 Acknowledgements

- Qt
- Flask
- gTTS
- PyMuPDF



