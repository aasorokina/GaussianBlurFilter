#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "filterimagewidget.h"
#include "gaussianfilter.h"
#include "sourceimgwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void setup_menu_bar();
  bool save_image(QString file_path, QString set_filter, QPixmap image);

private:
  QWidget *central_widget_;
  QMenuBar *menu_bar_;
  QMenu *file_menu_;
  QMenu *about_menu_;
  QAction *save_action_;
  QVBoxLayout *layout_;

  QTabWidget *tab_;
  SourceImgWidget *source_img_widget_;
  FilterImageWidget *filter_img_widget_;

private slots:
  void open_file_clicked();
  void save_file_clicked();
  void about_menu_clicked();
};
#endif // MAINWINDOW_H
