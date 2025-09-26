#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QObject>
#include <QList>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>

class TaskModel : public QObject
{
    Q_OBJECT

public:
    explicit TaskModel(QObject *parent = nullptr);

    // Методы для работы с задачами
    void addTask(const QString &text);
    void removeTask(int index);
    void setTaskDone(int index, bool done);
    void clearAll();
    void editTask(int index, const QString &newText);
    void moveTaskUp(int index);
    void moveTaskDown(int index);

    // Методы для получения данных
    int count() const;
    QString taskText(int index) const;
    bool isTaskDone(int index) const;

    // Сохранение и загрузка
    bool saveToJson(const QString &filename);
    bool loadFromJson(const QString &filename);


signals:
    // Сигналы об изменениях в данных
    void dataChanged();
    void taskAdded(int index);
    void taskRemoved(int index);
    void taskUpdated(int index);
    void taskMoved(int fromIndex, int toIndex);

private:
    struct Task {
        QString text;
        bool isDone = false;
    };

    QList<Task> m_tasks;
    static const QString DATA_FILE;
};

#endif // TASKMODEL_H
