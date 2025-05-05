#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "md5.h"
#include "sha256.h"
#include "scan.h"
#include <stdio.h>

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

QStringList fileNames;

int score = 0;
int num_patterns = 0;
scan::BytePattern patterns[MAX_PATTERNS];

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

void MainWindow::on_selectButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    if (!fileNames.isEmpty()) {
        qDebug() << fileNames.first();
    }
}


void MainWindow::on_scanButton_clicked()
{
    FILE *f = fopen(fileNames.first().toLocal8Bit().constData(), "rb");
    if (!f) {

        qDebug() << "slava zmeyu!";
        QApplication::quit();
    }


    md5::MD5Context ctx;
    md5::md5Init(&ctx);

    uint8_t buffer[4096];
    size_t total_bytes = 0;
    sha256_cl::sha256_context sha_ctx;
    sha256_cl::sha256_init(&sha_ctx);

    while (!feof(f)) {
        size_t bytes_read = fread(buffer, 1, 4096, f);
        if (bytes_read > 0) {
            md5::md5Update(&ctx, buffer, bytes_read);
            sha256_cl::sha256_hash(&sha_ctx, buffer, bytes_read);
            int k = scan::check_polymorphic(buffer, bytes_read);
            if (k==1){
                score = 3;
                qDebug() << "SCORE IS %d" << score;
                qDebug() << "We recommend to run this file on your own risk bc there's small risk of being virus";
                qDebug() << "ur file is polymorphic virus";
                fclose(f);
                QApplication::quit();
            }
            total_bytes += bytes_read;
        }
    }

    uint8_t hash[16];
    md5::md5Finalize(&ctx);
    memcpy(hash, ctx.digest, 16);

    //printf("read: %d\n", total_bytes);
    char* result = scan::ya_gomoseksualist_md5(hash);
    fclose(f);
    uint8_t hash_sha256[32];
    sha256_cl::sha256_done(&sha_ctx, hash);

    char* res = scan::ya_gomoseksualist_sha256(hash);
    qDebug() << "SHA256 hash is" << res;
    free(res);

    FILE* file_md5 = fopen("md5.txt", "a+");
    if (!file_md5) {
        qDebug() << "ZMEY BLYA!!!";
    }
    FILE* file_sha256 = fopen("sha256.txt", "a+");
    if (!file_sha256) {
        qDebug() << "ROMAN!!!";
    }
    char buff[256];
    qDebug() << "MD5 result is" << result;

    while (fgets(buff, sizeof(buff), file_md5)) {
        if (strncmp(buff, result, 32)==0){
            score = 10;
            qDebug() << "SCORE IS" << score;
            qDebug() << "Your file is virus it can shut down the whole your OS";
            fclose(file_md5);
            //printf("ur file seems to be virus file\n");
            QApplication::quit();
        }
    }

    while (fgets(buff, sizeof(buff), file_sha256)) {
        if (strncmp(buff, res, 64)==0){
            score = 10;
            qDebug() << "SCORE IS" << score;
            qDebug() << "Your file is virus it can shut down the whole your OS";
            fclose(file_sha256);
            //printf("ur file seems to be virus file\n");
            QApplication::quit();
        }
    }
    fclose(file_sha256);
    fclose(file_md5);
    int count = 0;

    score = 0;
    qDebug() << "Your score is" << score;
    qDebug() << "File is safe so u can run it without any concerns";
}

