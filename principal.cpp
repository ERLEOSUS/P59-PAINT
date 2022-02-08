#include "principal.h"
#include "ui_principal.h"

#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QInputDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::Principal)
{
   ui->setupUi(this);
   // Instanciando la imagen (creando)
   mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
   // Rellenar la imagen de color blanco
   mImagen->fill(Qt::white);
   // Instanciar el Painter a partir de la imagen
   mPainter = new QPainter(mImagen);
   mPainter->setRenderHint(QPainter::Antialiasing);
   // Inicializar otras variables
   mPuedeDibujar = false;
   mColor = Qt::black;
   mAncho = DEFAULT_ANCHO;
   mNumLineas = 0;
}

Principal::~Principal()
{
   delete ui;
   delete mPainter;
   delete mImagen;
}

void Principal::paintEvent(QPaintEvent *event)
{
   // Crear el painter de la ventana principal
   QPainter painter(this);
   // Dibujar la imagen
   painter.drawImage(0, 0, *mImagen);
   // Acepatr el evento
   event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
   mPuedeDibujar = true;
   mInicial = event->pos();
   event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{

   if(ui->actionLibre->isChecked()){

       // Validar si se puede dibujar
       if ( !mPuedeDibujar ) {
           event->accept();
           return;
       }
       // Capturar el punto donde se suelta el mouse
       mFinal = event->pos();
       // Crear un pincel y establecer atributos
       QPen pincel;
       pincel.setColor(mColor);
       pincel.setWidth(mAncho);
       // Dibujar una linea
       mPainter->setPen(pincel);
       mPainter->drawLine(mInicial,mFinal);
       // Mostrar el número de líneas en la barra de estado
       ui->statusbar->showMessage(tr("Número de líneas: ") + QString::number(++mNumLineas));
       // Actualizar la interfaz
       update();
       // actualizar el punto inicial
       mInicial = mFinal;

   }

   // Actualizar la interfaz
   update();
   // Aceptar el vento
   event->accept();


}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
   mPuedeDibujar = false;
   mFinal = event->pos();

   dibujar();

   // Actualizar la interfaz
   update();
   // Aceptar el vento
   event->accept();

}


void Principal::on_actionAncho_triggered()
{
   mAncho = QInputDialog::getInt(this,
                                 tr("Ancho del pincel"),
                                 tr("Ingrese el ancho del pincel de dibujo"),
                                 mAncho,
                                 1, 100);
}

void Principal::on_actionSalir_triggered()
{
   this->close();
}

void Principal::on_actionColor_triggered()
{
   mColor = QColorDialog::getColor(mColor,
                                   this,
                                   tr("Color del pincel"));
}

void Principal::on_actionNuevo_triggered()
{
   mImagen->fill(Qt::white);
   mNumLineas = 0;
   update();
}

void Principal::on_actionGuardar_triggered()
{
   QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                        tr("Guardar imagen"),
                                                        QString(),
                                                        tr("Imágenes (*.png)"));
   if ( !nombreArchivo.isEmpty() ){
       if (mImagen->save(nombreArchivo))
           QMessageBox::information(this,
                                    tr("Guardar imagen"),
                                    tr("Archivo almacenado en: ") + nombreArchivo);
       else
           QMessageBox::warning(this,
                                tr("Guardar imagen"),
                                tr("No se pudo almacenar la imagen."));
   }
}

void Principal::on_actionLineas_triggered()
{
   ui->actionLibre->setChecked(false);
   ui->actionCircunferencias->setChecked(false);
   ui->actionRect_nculos->setChecked(false);

}


void Principal::on_actionLibre_triggered()
{
   ui->actionLineas->setChecked(false);
   ui->actionCircunferencias->setChecked(false);
   ui->actionRect_nculos->setChecked(false);
}


void Principal::on_actionRect_nculos_triggered()
{
   ui->actionLineas->setChecked(false);
   ui->actionCircunferencias->setChecked(false);
   ui->actionLibre->setChecked(false);
}


void Principal::on_actionCircunferencias_triggered()
{
   ui->actionLineas->setChecked(false);
   ui->actionRect_nculos->setChecked(false);
   ui->actionLibre->setChecked(false);
}

void Principal::dibujar()
{

   if(ui->actionLineas->isChecked()){
       // Crear un pincel y establecer atributos
       QPen pincel;
       pincel.setColor(mColor);
       pincel.setWidth(mAncho);
       // Dibujar una linea
       mPainter->setPen(pincel);
       mPainter->drawLine(mInicial.x(),mInicial.y(),mFinal.x(),mFinal.y());
   }
   if(ui->actionCircunferencias->isChecked()){
       // Crear un pincel y establecer atributos
       QPen pincel;
       pincel.setColor(mColor);
       pincel.setWidth(mAncho);
       // Dibujar una linea
       mPainter->setPen(pincel);
       mPainter->drawEllipse(mInicial.x(),mInicial.y(),mFinal.x()-mInicial.x(), mFinal.y() - mInicial.y());
       //mPainter->drawEllipse(mInicial.x(),mInicial.y(),mFinal.x(), mFinal.y());

   }
   if(ui->actionRect_nculos->isChecked()){
       // Crear un pincel y establecer atributos
       QPen pincel;
       pincel.setColor(mColor);
       pincel.setWidth(mAncho);
       // Dibujar una linea
       mPainter->setPen(pincel);
       mPainter->drawRect(mInicial.x(),mInicial.y(),mFinal.x() - mInicial.x(), mFinal.y() - mInicial.y());
   }
}
