from flask import Flask, request, send_from_directory, jsonify
from flask_cors import CORS
from werkzeug.utils import secure_filename
from gtts import gTTS
import os
import PyPDF2

app = Flask(__name__)
CORS(app)

UPLOAD_FOLDER = 'uploads'
AUDIO_FOLDER = 'audio'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(AUDIO_FOLDER, exist_ok=True)

@app.route('/convert', methods=['POST'])
def convert():
    file = request.files['file']
    if not file or not file.filename.endswith('.pdf'):
        return jsonify({'error': 'Invalid file'}), 400

    filename = secure_filename(file.filename)
    file_path = os.path.join(UPLOAD_FOLDER, filename)
    file.save(file_path)

    # Extract text from PDF
    text = ""
    try:
        with open(file_path, 'rb') as pdf_file:
            reader = PyPDF2.PdfReader(pdf_file)
            for page in reader.pages:
                text += page.extract_text() or ""
    except Exception as e:
        return jsonify({'error': 'Failed to read PDF: ' + str(e)}), 500

    if not text.strip():
        return jsonify({'error': 'No text found in PDF'}), 400

    # Convert text to speech
    try:
        tts = gTTS(text=text)
        audio_filename = os.path.splitext(filename)[0] + '.mp3'
        audio_path = os.path.join(AUDIO_FOLDER, audio_filename)
        tts.save(audio_path)
    except Exception as e:
        return jsonify({'error': 'Text-to-speech failed: ' + str(e)}), 500

    return jsonify({'audio_file': audio_filename}), 200

@app.route('/audio/<filename>')
def serve_audio(filename):
    return send_from_directory(AUDIO_FOLDER, filename)

if __name__ == '__main__':
    app.run(debug=True)
