#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lastIndex = -1;

    QSettings settings;
    QStringList regexes = settings.value("regexes").toStringList();
    for (int ii = 0; ii < regexes.size(); ii+=2) {
        ui->comboBoxRegexes->addItem(regexes[ii], regexes[ii+1]);
    }

    int currentIndex = settings.value("currentRegex", -1).toInt();
    if (currentIndex >= 0) {
        ui->comboBoxRegexes->setCurrentIndex(currentIndex);
        lastIndex = currentIndex;
    }
}

MainWindow::~MainWindow()
{
    ui->comboBoxRegexes->setItemData(ui->comboBoxRegexes->currentIndex(), ui->plainTextEditRegex->toPlainText());

    QStringList s;
    for (int ii = 0; ii < ui->comboBoxRegexes->count(); ++ii) {
        s << ui->comboBoxRegexes->itemText(ii) << ui->comboBoxRegexes->itemData(ii).toString();
    }

    QSettings settings;
    settings.setValue("regexes", s);
    settings.setValue("currentRegex", ui->comboBoxRegexes->currentIndex());

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
            QMessageBox::critical(this, "", tr("Must have \"===\""));
            return;
        }

        QRegExp regex(s[0]);
        if (!regex.isValid()) {
            QMessageBox::critical(this, "", tr("Invalid regex: %1").arg(s[0]));
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

void MainWindow::on_comboBoxRegexes_currentIndexChanged(int index)
{
    Q_ASSERT(index == ui->comboBoxRegexes->currentIndex());
    if (lastIndex >= 0) {
        ui->comboBoxRegexes->setItemData(lastIndex, ui->plainTextEditRegex->toPlainText());
    }
    ui->plainTextEditRegex->setPlainText(ui->comboBoxRegexes->currentData().toString());
    lastIndex = index;
}
