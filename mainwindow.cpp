#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings;
    ui->lineEditCurrentRegex->setText(settings.value("currentRegex").toString());
    ui->plainTextEditRegex->setPlainText(settings.value("regexes/" + ui->lineEditCurrentRegex->text()).toString());
}

MainWindow::~MainWindow()
{
    if (!ui->lineEditCurrentRegex->text().isEmpty()) {
        QSettings settings;
        settings.setValue("currentRegex", ui->lineEditCurrentRegex->text());
        settings.setValue("regexes/"+ui->lineEditCurrentRegex->text(), ui->plainTextEditRegex->toPlainText());
    }
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString input = ui->plainTextEditInput->toPlainText();
    QString output = input;
    QString regexes = ui->plainTextEditRegex->toPlainText();
    foreach (QString line, regexes.split('\n')) {
        QStringList s = line.split("===");
        if (s.size() != 2) {
            return;
        }

        QRegExp regex(s[0]);
        if (!regex.isValid()) {
            return;
        }

        QString after(s[1]);

        s = output.split('\n');
        output.clear();
        foreach (QString line, s) {
            line.replace(regex, after);
            output += line + '\n';
        }
    }

    ui->plainTextEditOutput->setPlainText(output);
}
