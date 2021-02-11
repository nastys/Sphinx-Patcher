/*
 * Copyright © 2021 nastys
 *
 * This file is part of Sphinx Patcher.
 * Sphinx Patcher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sphinx Patcher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sphinx Patcher.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <qt-crc32/crc32.h>
#include <QSysInfo>
#include <QDesktopServices>
#include "patcher.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_patch_clicked()
{
    switch(ui->tabWidget->currentIndex())
    {
    case 0:
        patch_xbox();
        break;
    case 4:
        patch_switch();
    }
}

void MainWindow::patch_switch()
{
    QDesktopServices::openUrl(QUrl("https://github.com/nastys/Sphinx-NX-IPS-Patches"));
}

void MainWindow::patch_xbox()
{
    QString inputfilepath = QFileDialog::getOpenFileName(this, "Open original default.xbe", "", "default.xbe");
    if(inputfilepath.isNull()) return;

    QString bakfilepath = inputfilepath + ".bak";
    bool usebak = false;
    unsigned char version = 0;
    if(!(version = verify_xbox(inputfilepath)))
    {
        usebak = true;
        if(!(version = verify_xbox(bakfilepath)))
        {
            QMessageBox::critical(this, "Error", "This version is not supported or the file has been modified.");
            return;
        }
    }

    if(usebak)
    {
        QMessageBox::StandardButton diag = QMessageBox::warning(this, "Warning", "default.xbe will be overwritten.\n\nContinue?", QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
        if(!(diag == QMessageBox::StandardButton::Yes)) return;
        if(!QFile::remove(inputfilepath))
        {
            QMessageBox::critical(this, "Error", "Cannot delete default.xbe.");
            return;
        }
    }
    else
    {
        QFile::remove(bakfilepath);
        if(!QFile::rename(inputfilepath, bakfilepath))
        {
            QMessageBox::critical(this, "Error", "Cannot rename default.xbe.");
            return;
        }
    }

    QFile outputfile(inputfilepath);
    if(!(outputfile.open(QIODevice::ReadWrite)&&outputfile.resize(0)))
    {
        outputfile.remove();
        QMessageBox::critical(this, "Error", "Cannot create default.xbe.");
        return;
    }

    QFile bakfile(bakfilepath);
    bakfile.open(QIODevice::ReadOnly);
    QByteArray array = bakfile.readAll();

    bool isPal=version==2;

    if(ui->radioButton_shadows_simple->isChecked()||ui->radioButton_shadows_none->isChecked())
        writePatch(array, 0x1785D0, {0x00}, 0x0F9E85, {0x00}, isPal);
    if(ui->radioButton_shadows_none->isChecked())
        writePatch(array, 0x178469, {0x00}, 0x0F9D1E, {0x00}, isPal);

    if(ui->radioButton_aspect_system->isChecked())
        writePatch(array, 0x13819A, {0xEB}, 0x0CA9BA, {0xEB}, isPal);
    if(ui->radioButton_aspect_normal->isChecked()||ui->radioButton_aspect_wide->isChecked())
        writePatch(array, 0x1381AA, {0x90, 0x90}, 0x0CA9CA, {0x90, 0x90}, isPal);
    if(ui->radioButton_aspect_wide->isChecked())
        writePatch(array, 0x1381AC, {0xB3, 0x01}, 0x0CA9CC, {0xB3, 0x01}, isPal);

    if(ui->checkBox_widescreen_aspect->isChecked())
    {
        QVector<unsigned char> newRatioQVector = ratioToBytes(ui->lineEdit_ratio->text(), 1.6, 1.0);

        writePatch(array, 0x20C95B, {0x4E, 0x9D, 0x3A}, 0x16222B, {0xAE, 0x09, 0x25}, isPal);
        writePatch(array, 0x20CE2E, {0x4E, 0x9D, 0x3A}, 0x1626FE, {0xAE, 0x09, 0x25}, isPal);
        writePatch(array, 0x385FED, {0x00}, 0x240D4D, {0x00}, isPal);
        writePatch(array, 0x385FEE, newRatioQVector, 0x240D4E, newRatioQVector, isPal);
    }

    if(ui->checkBox_widescreen_2dwidth->isChecked())
    {
        QVector<unsigned char> newWidthQVector = ratioToBytes(ui->lineEdit_width->text(), 1.33);

        writePatch(array, 0x36E724, newWidthQVector, 0x22D1F0, newWidthQVector, isPal);
    }

    outputfile.write(array);
    outputfile.close();

    QMessageBox::information(this, "Patch Xbox", "Done!");
}

unsigned char MainWindow::verify_xbox(QString& path)
{
    Crc32 crc32func;
    quint32 crc32 = crc32func.calculateFromFile(path);

    if(crc32 == 0x2FFF36C) // NTSC version
        return 1;
    else if(crc32 == 0x370A601C) // PAL version
        return 2;

    return 0;
}

void MainWindow::on_checkBox_widescreen_aspect_stateChanged(int arg1)
{
    ui->lineEdit_ratio->setEnabled(arg1);
}

void MainWindow::on_checkBox_widescreen_2dwidth_stateChanged(int arg1)
{
    ui->lineEdit_width->setEnabled(arg1);
}
