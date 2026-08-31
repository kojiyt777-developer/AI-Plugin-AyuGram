#include <QObject>
#include <QString>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class AiPlugin : public QObject {
    Q_OBJECT
public:
    explicit AiPlugin(QObject* parent = nullptr) : QObject(parent) {
        manager = new QNetworkAccessManager(this);
    }
    
    QString getName() const { return "AI Chat"; }
    QString getVersion() const { return "1.0"; }
    
    void onMessage(const QString& chatId, const QString& userId, const QString& text) {
        if (text.startsWith(".ai")) {
            QString prompt = text.mid(3).trimmed();
            if (prompt.isEmpty()) {
                prompt = "Расскажи о себе";
            }
            
            // Отправляем ответ
            askAi(chatId, prompt);
        }
    }
    
    void askAi(const QString& chatId, const QString& prompt) {
        // URL Ollama (по умолчанию localhost)
        QUrl url("http://localhost:11434/api/generate");
        
        QJsonObject body;
        body["model"] = "llama2";
        body["prompt"] = prompt;
        body["stream"] = false;
        
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        
        QNetworkReply* reply = manager->post(request, QJsonDocument(body).toJson());
        
        connect(reply, &QNetworkReply::finished, [this, reply, chatId]() {
            reply->deleteLater();
            
            if (reply->error() != QNetworkReply::NoError) {
                sendMessage(chatId, "Ошибка: Ollama не запущен. Установите с ollama.ai");
                return;
            }
            
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QString answer = doc.object()["response"].toString().trimmed();
            
            if (answer.isEmpty()) {
                answer = "Не удалось получить ответ";
            }
            
            sendMessage(chatId, answer);
        });
    }
    
    void sendMessage(const QString& chatId, const QString& text) {
        // Заглушка — здесь будет вызов API AyuGram
        qDebug() << "SEND to" << chatId << ":" << text;
    }

private:
    QNetworkAccessManager* manager;
};

extern "C" {
    AiPlugin* createPlugin() {
        return new AiPlugin();
    }
    
    void destroyPlugin(AiPlugin* plugin) {
        delete plugin;
    }
}
