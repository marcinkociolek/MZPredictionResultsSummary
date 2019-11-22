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
    if (!exists(FileFolder))
    {
        return;
    }
    if (!is_directory(FileFolder))
    {
        return;
    }
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
string EvaluatePrediction(path PredictedFileToProcess)
{
    string OutStr = "";
    if(!exists(PredictedFileToProcess))
        return "file" + PredictedFileToProcess.string() + "does not exists";

    //std::vector<int> PredictedIntVector;
    //std::vector<std::string> PredictedStdVector;
    //std::vector<std::string> TrueStdVector;

    std::ifstream inFile(PredictedFileToProcess.string());
    if (!inFile.is_open())
    {
        return "file" + PredictedFileToProcess.string() + "unable to open";
    }
    // ------------------read params from file-----------------------------

    string Line;


    int samplesCount = 0;
    int errorCount = 0;
    size_t position;
    while(inFile.good())
    {
        samplesCount++;

        getline(inFile,Line);

        if(Line == "")
            break;

        position = Line.find(" ");
        string predictedInt = Line.substr(0,position);
        Line = Line.substr(position + 1);

        position = Line.find(" : ");
        string predictedStr = Line.substr(0,position);
        Line = Line.substr(position + 3);

        position = Line.find(" ");
        string trueStr = Line.substr(0,position);


        string Result;
        if(predictedStr == trueStr)
        {
            Result = " OK";
        }
        else
        {
            Result = " False";
            errorCount++;
        }

        OutStr += to_string(samplesCount)
                  + "-" + predictedInt
                  + "-" + predictedStr
                  + "-" + trueStr
                  + "-" + Result
                  + "\n";

    }
    OutStr += "\n";
    OutStr += "-----------------------\n";
    OutStr += "     Summatu\n";
    OutStr += "-----------------------\n";

    OutStr += "Samples Count " + to_string(samplesCount) + "\n";
    OutStr += "Error Count " + to_string(errorCount) + "\n";
    if(samplesCount)
    {
       double errorPerc = (double)errorCount/(double)samplesCount * 100.0;
       OutStr += "Error % " + to_string(errorPerc) + "\n";
    }
    else
        OutStr += "Error % -1\n";


    inFile.close();
    return OutStr;
}

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
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::OpenPredictorOutputFotder()
{
    if (!exists(PredictorOutputFolder))
    {
        ui->textEditOut->append(QString::fromStdString("Predictor Output folder : " + PredictorOutputFolder.string()+ " not exists "));
    }
    if (!is_directory(PredictorOutputFolder))
    {
        ui->textEditOut->append(QString::fromStdString( "Predicto rOutput folder : " + PredictorOutputFolder.string()+ " This is not a directory path "));
    }
    ui->lineEditPredictorOutputFolder->setText(QString::fromStdString(PredictorOutputFolder.string()));
    ReadFolder(PredictorOutputFolder, &PredictorOutputFileNamesVector);
    ui->textEditPredictorOutFiles->clear();
    ui->textEditPredictorOutFiles->append(QString::fromStdString(StringVectorToString(PredictorOutputFileNamesVector)));
    int nrOfFiles = PredictorOutputFileNamesVector.size();
    ui->spinBoxPredictedFileToShow->setMaximum(nrOfFiles-1);
    ShowResult();

}
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::OutputFotder()
{
    if (!exists(OutputFolder))
    {
        ui->textEditOut->append(QString::fromStdString("Output folder : " + OutputFolder.string()+ " not exists "));
    }
    if (!is_directory(OutputFolder))
    {
        ui->textEditOut->append(QString::fromStdString( "Output folder : " + PredictorOutputFolder.string()+ " This is not a directory path "));
    }
    ui->lineEditPredictorOutputFolder->setText(QString::fromStdString(PredictorOutputFolder.string()));

}
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowResult()
{

    if(!PredictorOutputFileNamesVector.size())
        return;
    unsigned long long fileNrToProcess = ui->spinBoxPredictedFileToShow->value();
    path PredictorOutputFile = PredictorOutputFolder;
    PredictorOutputFile.append(PredictorOutputFileNamesVector[fileNrToProcess]);
    string OutText =  EvaluatePrediction(PredictorOutputFile);
    ui->textEditOutFileContent->clear();
    ui->textEditOutFileContent->setText(QString::fromStdString(OutText));

}
//------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          Slots
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_pushButtonOpenPredictorOutputFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(QString::fromStdString(PredictorOutputFolder.string()));

    if(dialog.exec())
    {
        PredictorOutputFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenPredictorOutputFotder();
}

void MainWindow::on_pushButtonOpenOutFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(QString::fromStdString(OutputFolder.string()));

    if(dialog.exec())
    {
        OutputFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OutputFotder();
}

void MainWindow::on_spinBoxPredictedFileToShow_valueChanged(int arg1)
{
    ShowResult();
}
