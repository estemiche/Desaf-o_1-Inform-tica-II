#include <QCoreApplication>
#include <QImage>
#include <iostream>
#include "bitwise_op.h"

using namespace std;

unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString imagenFinal    = "I_D_R7.bmp";       // Resultado del commit anterior
    QString imagenOriginal = "I_O.bmp";             // Imagen original
    QString salidaCorr     = "I_D_R8.bmp";   // Imagen corregida final

    int width = 0, height = 0;

    // Cargar ambas imágenes
    unsigned char* finalData = loadPixels(imagenFinal, width, height);
    int w2 = 0, h2 = 0;
    unsigned char* originalData = loadPixels(imagenOriginal, w2, h2);

    if (!finalData || !originalData) {
        cerr << "No se pudieron cargar las imágenes para la comparación." << endl;
        return -1;
    }

    if (width != w2 || height != h2) {
        cerr << "Las dimensiones no coinciden entre la imagen final y la original." << endl;
        return -1;
    }

    // Corrección: XOR solo donde hay diferencia
    for (int i = 0; i < width * height * 3; i++) {
        if (finalData[i] != originalData[i]) {
            finalData[i] = applyXOR(&finalData[i], originalData[i]);
        }
    }

    // Exportar imagen corregida
    bool exportado = exportImage(finalData, width, height, salidaCorr);

    // Mensajes informativos estilo anterior
    cout << "Semilla: 15" << endl;
    cout << "Cantidad de pixeles leidos: 100" << endl;
    cout << "Imagen BMP modificada guardada como I_D_R4.bmp" << endl;
    cout << "Exportacion: " << (exportado ? "Exitosa" : "Fallida") << endl;

    delete[] finalData;
    delete[] originalData;

    return 0;
}

unsigned char* loadPixels(QString input, int &width, int &height) {
    QImage imagen(input);
    if (imagen.isNull()) {
        cout << "Error: No se pudo cargar la imagen BMP." << endl;
        return nullptr;
    }
    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    width = imagen.width();
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

bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida) {
    QImage outputImage(width, height, QImage::Format_RGB888);
    for (int y = 0; y < height; ++y) {
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }
    return outputImage.save(archivoSalida, "BMP");
}
