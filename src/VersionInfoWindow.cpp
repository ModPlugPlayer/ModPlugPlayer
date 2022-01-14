#include "VersionInfoWindow.hpp"
#include "ui_VersionInfoWindow.h"
#include <QSysInfo>
#include <fftw3.h>
#include <libopenmpt/libopenmpt.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>

int getBitness() {
    return sizeof(void*)*8;
}

QString getCompilerVersion() {
    #if defined __clang__
        #if defined __APPLE__
            return __VERSION__;
        #else
            #if __clang_major__ < 3
                return "Clang " + QString(__clang_major__);
            #else
                return QString("Clang %1.%2.%3 %4-Bit").arg(__clang_major__).arg(__clang_minor__).arg(__clang_patchlevel__).arg(getBitness());
            #endif
        #endif
    #elif defined __GNUC__
        return QString("GCC %1 %2-Bit").arg(__VERSION__).arg(getBitness());
    #elif defined _MSC_FULL_VER
        return "Microsoft Visual C++ Version " + QString::number(_MSC_FULL_VER) + QString::number(getBitness()) + "-Bit";
    #elif defined _MSC_VER
       return "Microsoft Visual C++ Version " + QString::number(_MSC_VER) + QString::number(getBitness()) + "-Bit";
    #else
        return "Other Compiler"
    #endif
}

VersionInfoWindow::VersionInfoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionInfoWindow)
{
    ui->setupUi(this);
    ui->labelCompiler->setText(getCompilerVersion());
    ui->labelBuildCpuArchitecture->setText(QSysInfo::buildCpuArchitecture());
    ui->labelKernelType->setText(QSysInfo::kernelType());
    ui->labelQt->setText(qVersion());
    ui->labelLibFFTW->setText(fftw_version);
    ui->labelLibOpenMPT->setText(QString::fromStdString(openmpt::string::get("library_version")));
    ui->labelLibPortAudio->setText(QString("%1.%2.%3").arg(Pa_GetVersionInfo()->versionMajor).arg(Pa_GetVersionInfo()->versionMinor).arg(Pa_GetVersionInfo()->versionSubMinor));
}

VersionInfoWindow::~VersionInfoWindow()
{
    delete ui;
}
