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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_patch_clicked();

    void on_checkBox_widescreen_aspect_stateChanged(int arg1);

    void on_checkBox_widescreen_2dwidth_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    void patch_xbox();
    void patch_switch();
    unsigned char verify_xbox(QString& path);
};
#endif // MAINWINDOW_H
