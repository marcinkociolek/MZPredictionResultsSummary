#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <boost/filesystem.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    boost::filesystem::path QMaZdaFolder;
    boost::filesystem::path ImageFolder;
    boost::filesystem::path ROIFolder;
    boost::filesystem::path OptionsFolder;
    boost::filesystem::path MzFeaturesOutFolder;
    boost::filesystem::path ClassyfiersFolder;
    boost::filesystem::path ClassyfiersOptionFile;
    boost::filesystem::path PredictorOutputFolder;
    boost::filesystem::path BatFolder;

    std::string OutFilePrefix;
    std::string BatFileName;
    std::string FilePattern;

    std::vector<std::string> ImageFileNamesVector;
    std::vector<std::string> RoiFileNamesVector;
    std::vector<std::string> OptionsFileNamesVector;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
