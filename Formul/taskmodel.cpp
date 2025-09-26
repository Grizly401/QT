#include "taskmodel.h"

const QString TaskModel::DATA_FILE = "tasks.json";

TaskModel::TaskModel(QObject *parent) : QObject(parent) {}

// Добавление новой задачи
void TaskModel::addTask(const QString &text)
{
    Task newTask;
    newTask.text = text;
    newTask.isDone = false;

    m_tasks.append(newTask);
    emit taskAdded(m_tasks.count() - 1); // Сигнал о добавлении
    emit dataChanged(); // Общий сигнал об изменениях
}

// Удаление задачи
void TaskModel::removeTask(int index)
{
    if (index < 0 || index >= m_tasks.count()) return;

    m_tasks.removeAt(index);
    emit taskRemoved(index); // Сигнал об удалении
    emit dataChanged();
}

// Изменение статуса задачи
void TaskModel::setTaskDone(int index, bool done)
{
    if (index < 0 || index >= m_tasks.count()) return;

    m_tasks[index].isDone = done;
    emit taskUpdated(index); // Сигнал об изменении
    emit dataChanged();
}

// Редактирование текста задачи
void TaskModel::editTask(int index, const QString &newText)
{
    if (index < 0 || index >= m_tasks.count()) return;

    m_tasks[index].text = newText;
    emit taskUpdated(index);
    emit dataChanged();
}

// Очистка всех задач
void TaskModel::clearAll()
{
    m_tasks.clear();
    emit dataChanged();
}

// Получение количества задач
int TaskModel::count() const
{
    return m_tasks.count();
}

// Получение текста задачи
QString TaskModel::taskText(int index) const
{
    if (index < 0 || index >= m_tasks.count()) return "";
    return m_tasks[index].text;
}

// Проверка статуса задачи
bool TaskModel::isTaskDone(int index) const
{
    if (index < 0 || index >= m_tasks.count()) return false;
    return m_tasks[index].isDone;
}

// Сохранение в JSON
bool TaskModel::saveToJson(const QString &filename)
{
    QJsonArray itemsArray;

    for (const Task &task : m_tasks) {
        QJsonObject itemObject;
        itemObject["text"] = task.text;
        itemObject["checked"] = task.isDone;
        itemsArray.append(itemObject);
    }

    QJsonObject rootObject;
    rootObject["items"] = itemsArray;
    rootObject["lastSaved"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonDocument doc(rootObject);
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }
    return false;
}

// Загрузка из JSON
bool TaskModel::loadFromJson(const QString &filename)
{
    QFile file(filename);
    if (!file.exists()) return false;

    if (!file.open(QIODevice::ReadOnly)) return false;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) return false;

    m_tasks.clear();
    QJsonArray itemsArray = doc.object()["items"].toArray();

    for (const QJsonValue &value : itemsArray) {
        QJsonObject itemObject = value.toObject();
        Task task;
        task.text = itemObject["text"].toString();
        task.isDone = itemObject["checked"].toBool();
        m_tasks.append(task);
    }

    emit dataChanged();
    return true;
}

void TaskModel::moveTaskUp(int index)
{
    if (index <= 0 || index >= m_tasks.count()) return;

    // Меняем местами задачи в модели данных
    m_tasks.move(index, index - 1);

    emit taskMoved(index, index - 1); // Новый сигнал
    emit dataChanged();
}

void TaskModel::moveTaskDown(int index)
{
    if (index < 0 || index >= m_tasks.count() - 1) return;

    // Меняем местами задачи в модели данных
    m_tasks.move(index, index + 1);

    emit taskMoved(index, index + 1); // Новый сигнал
    emit dataChanged();
}
