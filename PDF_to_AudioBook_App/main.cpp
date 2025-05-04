#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QFrame>

QMediaPlayer *player = nullptr;
QAudioOutput *audioOutput = nullptr;

void convertPdfToAudio(const QString &filePath, QLabel *statusLabel) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request(QUrl("http://127.0.0.1:5000/convert"));

    QFile *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        statusLabel->setText("❌ Failed to open the file.");
        return;
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" + file->fileName() + "\""));
    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(filePart);

    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QString audioFileName = QString(response).split("\"audio_file\": \"").value(1).split("\"").value(0);
            QString audioUrl = "http://127.0.0.1:5000/audio/" + audioFileName;

            // Download audio file
            QNetworkAccessManager *downloadManager = new QNetworkAccessManager();
            QNetworkReply *downloadReply = downloadManager->get(QNetworkRequest(QUrl(audioUrl)));

            QObject::connect(downloadReply, &QNetworkReply::finished, [=]() {
                if (downloadReply->error() == QNetworkReply::NoError) {
                    QFile output("downloaded_audio.mp3");
                    if (output.open(QIODevice::WriteOnly)) {
                        output.write(downloadReply->readAll());
                        output.close();
                        statusLabel->setText("✅ Converted and downloaded. Playing now...");

                        if (!player) {
                            player = new QMediaPlayer;
                            audioOutput = new QAudioOutput;
                            player->setAudioOutput(audioOutput);
                        }

                        player->setSource(QUrl::fromLocalFile("downloaded_audio.mp3"));
                        audioOutput->setVolume(1.0);  // full volume
                        player->play();

                    } else {
                        statusLabel->setText("❌ Failed to save the audio.");
                    }
                } else {
                    statusLabel->setText("❌ Audio download failed: " + downloadReply->errorString());
                }
                downloadReply->deleteLater();
                downloadManager->deleteLater();
            });

        } else {
            statusLabel->setText("❌ Conversion failed: " + reply->errorString());
        }

        reply->deleteLater();
        manager->deleteLater();
    });
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget mainWindow;
    mainWindow.setWindowTitle("🎧 PDF to Audio Converter");
    mainWindow.setStyleSheet("background-color: #f4f4f4;");
    mainWindow.resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    QLabel *titleLabel = new QLabel("🎧 PDF to Audio Converter");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #2c3e50;");
    layout->addWidget(titleLabel);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);

    QPushButton *chooseButton = new QPushButton("📂 Choose PDF File");
    chooseButton->setStyleSheet("font-size: 20px; color: white; background-color: #3498db; padding: 10px; border-radius: 10px;");
    layout->addWidget(chooseButton);

    QLabel *fileNameLabel = new QLabel("");
    fileNameLabel->setAlignment(Qt::AlignCenter);
    fileNameLabel->setStyleSheet("font-size: 16px; color: #34495e;");
    layout->addWidget(fileNameLabel);

    QPushButton *convertButton = new QPushButton("🔊 Convert to Audio");
    convertButton->setStyleSheet("font-size: 20px; color: white; background-color: #e74c3c; padding: 10px; border-radius: 10px;");
    convertButton->setVisible(false);  // Hide initially
    layout->addWidget(convertButton);

    QLabel *statusLabel = new QLabel("");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 16px; color: #e67e22;");
    layout->addWidget(statusLabel);

    QObject::connect(chooseButton, &QPushButton::clicked, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&mainWindow, "Choose PDF File", "", "PDF Files (*.pdf)");
        if (!filePath.isEmpty()) {
            fileNameLabel->setText(filePath);
            convertButton->setVisible(true);  // Show Convert button
        }
    });

    QObject::connect(convertButton, &QPushButton::clicked, [&]() {
        QString filePath = fileNameLabel->text();
        if (!filePath.isEmpty()) {
            statusLabel->setText("⏳ Converting...");
            convertPdfToAudio(filePath, statusLabel);
        } else {
            statusLabel->setText("⚠️ Please choose a PDF file first.");
        }
    });

    mainWindow.show();
    return app.exec();
}
