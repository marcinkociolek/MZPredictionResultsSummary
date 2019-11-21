#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <string>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using namespace boost;
using namespace std;
using namespace boost::filesystem;
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          My functions outside the Mainwindow class
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
void ReadFolder(path FileFolder, std::vector<std::string> *FileNamesVector, string FilePaternStr = ".+")
{
    FileNamesVector->clear();
    regex FilePattern(FilePaternStr);
    for (directory_entry& FileToProcess : directory_iterator(FileFolder))
    {

        if (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern ))
            continue;
        path PathLocal = FileToProcess.path();

        FileNamesVector->push_back(PathLocal.filename().string());
    }
    std::sort(FileNamesVector->begin(),FileNamesVector->end());
}
//------------------------------------------------------------------------------------------------------------------------------
string StringVectorToString(std::vector<std::string> FileNamesVector)
{
    string OutStr = "";
    if(FileNamesVector.empty())
        return OutStr;
    for(vector<string>::iterator iFileNamesVector = FileNamesVector.begin(); iFileNamesVector != FileNamesVector.end(); iFileNamesVector++)
    {
        OutStr += *iFileNamesVector + "\n";
    }

    return OutStr;
}

//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          My functions in the Mainwindow class
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
