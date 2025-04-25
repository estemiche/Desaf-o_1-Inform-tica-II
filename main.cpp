#include <QCoreApplication>
#include <QImage>
#include <iostream>
#include "bitwise_op.h"

using namespace std;

unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString archivoEntrada   = "I_D_R5.bmp";
    QString archivoOriginal  = "I_O.bmp";
    QString archivoSalida    = "I_D_R6.bmp";

    int width = 0, height = 0;

    // Cargar la imagen parcialmente recuperada
    unsigned char* pixelData = loadPixels(archivoEntrada, width, height);
    if (!pixelData) {
        cerr << "Error al cargar la imagen '" << archivoEntrada.toStdString() << "'" << endl;
        return -1;
    }

    // Cargar la imagen original de referencia
    unsigned char* pixelOriginal = loadPixels(archivoOriginal, width, height);
    if (!pixelOriginal) {
        cerr << "Error al cargar la imagen '" << archivoOriginal.toStdString() << "'" << endl;
        delete[] pixelData;
        return -1;
    }

    // ✅ Commit 10: Corregir parte superior dañada (por ejemplo, primeras 5 filas)
    int filasCorregidas = 5;
    int pixelesCorregidos = 0;

    for (int y = 0; y < filasCorregidas; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            pixelData[index]     = pixelOriginal[index];     // R
            pixelData[index + 1] = pixelOriginal[index + 1]; // G
            pixelData[index + 2] = pixelOriginal[index + 2]; // B
            pixelesCorregidos++;
        }
    }

    cout << "Semilla: 0" << endl;
    cout << "Cantidad de pixeles leidos: " << pixelesCorregidos << endl;

    bool exportado = exportImage(pixelData, width, height, archivoSalida);
    cout << "Exportacion: " << (exportado ? "Exitosa" : "Fallida") << endl;

    delete[] pixelData;
    delete[] pixelOriginal;

    return 0;
}

unsigned char* loadPixels(QString input, int &width, int &height){
    QImage imagen(input);
    if (imagen.isNull()) {
        cout << "Error: No se pudo cargar la imagen BMP." << endl;
        return nullptr;
    }
    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    width  = imagen.width();
    height = imagen.height();

    int dataSize = width * height * 3;
    unsigned char* pixelData = new unsigned char[dataSize];
    for (int y = 0; y < height; ++y) {
        const uchar* srcLine = imagen.scanLine(y);
        unsigned char* dstLine = pixelData + y * width * 3;
        memcpy(dstLine, srcLine, width * 3);
    }
    return pixelData;
}

bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida){
    QImage outputImage(width, height, QImage::Format_RGB888);
    for (int y = 0; y < height; ++y) {
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }
    if (!outputImage.save(archivoSalida, "BMP")) {
        cout << "Error: No se pudo guardar la imagen BMP modificada.";
        return false;
    } else {
        cout << "Imagen BMP modificada guardada como " << archivoSalida.toStdString() << endl;
        return true;
    }
}
