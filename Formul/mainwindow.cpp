#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QListWidgetItem>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_taskModel(new TaskModel(this)) // Создаем модель
{
    ui->setupUi(this);

    // Загружаем данные
    m_taskModel->loadFromJson("tasks.json");

    // Настраиваем соединения
    setupConnections();

    // Первоначальное обновление интерфейса
    updateView();
}

MainWindow::~MainWindow()
{
    delete ui;
    // Модель удалится автоматически, т.к. parent = this
}

void MainWindow::setupConnections()
{
    // Соединяем сигналы модели с нашими слотами
    connect(m_taskModel, &TaskModel::dataChanged, this, &MainWindow::updateView);

    // Соединяем изменение элементов списка
    connect(ui->listWidget, &QListWidget::itemChanged, this, &MainWindow::onItemChanged);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_taskModel->saveToJson("tasks.json");
    event->accept();
}

// Обновление всего интерфейса
void MainWindow::updateView()
{
    // Блокируем сигналы чтобы не вызывать onItemChanged при программном изменении
    ui->listWidget->blockSignals(true);

    ui->listWidget->clear();

    for (int i = 0; i < m_taskModel->count(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(m_taskModel->taskText(i));

        // Устанавливаем флаги
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
        item->setCheckState(m_taskModel->isTaskDone(i) ? Qt::Checked : Qt::Unchecked);

        // Применяем стиль для выполненных задач
        if (m_taskModel->isTaskDone(i)) {
            QFont font = item->font();
            font.setStrikeOut(true);
            item->setFont(font);
            item->setForeground(Qt::gray);
        } else {
            QFont font = item->font();
            font.setStrikeOut(false);
            item->setFont(font);
            item->setForeground(Qt::black);
        }

        ui->listWidget->addItem(item);
    }

    // Разблокируем сигналы
    ui->listWidget->blockSignals(false);
}

// Обработка изменения элемента списка (чекбокс или редактирование)
void MainWindow::onItemChanged(QListWidgetItem *item)
{
    if (!item) return;

    // Блокируем сигналы чтобы избежать рекурсии
    ui->listWidget->blockSignals(true);

    // Получаем текст и состояние ДО каких-либо изменений
    QString itemText = item->text();
    Qt::CheckState checkState = item->checkState();

    // Применяем стиль сразу
    QFont font = item->font();
    font.setStrikeOut(checkState == Qt::Checked);
    item->setFont(font);
    item->setForeground(checkState == Qt::Checked ? Qt::gray : Qt::black);

    // Ищем соответствующую задачу в модели по тексту
    // (это более надежно, чем по индексу)
    int modelIndex = -1;
    for (int i = 0; i < m_taskModel->count(); ++i) {
        if (m_taskModel->taskText(i) == itemText) {
            modelIndex = i;
            break;
        }
    }

    if (modelIndex != -1) {
        // Определяем что изменилось
        if (checkState != (m_taskModel->isTaskDone(modelIndex) ? Qt::Checked : Qt::Unchecked)) {
            // Изменился чекбокс
            m_taskModel->setTaskDone(modelIndex, checkState == Qt::Checked);
        } else if (itemText != m_taskModel->taskText(modelIndex)) {
            // Изменился текст
            m_taskModel->editTask(modelIndex, itemText);
        }
    }

    // Разблокируем сигналы
    ui->listWidget->blockSignals(false);
}

void MainWindow::on_pushButton_Add_clicked()
{
    QString task = ui->lineEdit->text();
    if (!task.isEmpty()) {
        m_taskModel->addTask(task); // Вызываем метод модели
        ui->lineEdit->clear();
    }
}

void MainWindow::on_pushButton_Delete_clicked()
{
    int row = ui->listWidget->currentRow();
    if (row != -1) {
        m_taskModel->removeTask(row); // Вызываем метод модели
    }
}

void MainWindow::on_Clear_All_clicked()
{
    if (m_taskModel->count() == 0) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Очистка списка",
                                  "Вы уверены, что хотите очистить весь список?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_taskModel->clearAll(); // Вызываем метод модели
    }
}

void MainWindow::on_pushButton_Up_toList_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow > 0) {
        m_taskModel->moveTaskUp(currentRow); // Вызываем метод модели!
        // View автоматически обновится через сигнал dataChanged()
    }

}


void MainWindow::on_pushButton_Down_toList_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow < ui->listWidget->count() - 1) {
        m_taskModel->moveTaskDown(currentRow); // Вызываем метод модели!
        // View автоматически обновится через сигнал dataChanged()
    }
}

