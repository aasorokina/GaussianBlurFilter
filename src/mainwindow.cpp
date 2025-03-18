#include "../include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  resize(800, 600);
  setWindowIcon(QIcon(":/img/img/app_icon.png"));
  setWindowTitle("Gaussian blur");
  central_widget_ = new QWidget(this);
  layout_ = new QVBoxLayout(central_widget_);

  setup_menu_bar();

  source_img_widget_ = new SourceImgWidget(this);
  filter_img_widget_ = new FilterImageWidget(this);

  tab_ = new QTabWidget(this);
  tab_->setTabPosition(QTabWidget::North);
  tab_->addTab(source_img_widget_, "Изображение");
  tab_->addTab(filter_img_widget_, "Фильтр");
  layout_->addWidget(tab_);

  setCentralWidget(central_widget_);
}

MainWindow::~MainWindow() {}

void MainWindow::setup_menu_bar() {
  menu_bar_ = new QMenuBar(this);

  file_menu_ = new QMenu("&Файл", menu_bar_);

  file_menu_->addAction(QIcon(":/img/img/open_icon.png"), "&Открыть", this,
                        &MainWindow::open_file_clicked, Qt::CTRL | Qt::Key_O);
  file_menu_->addAction(QIcon(":/img/img/save_icon.png"), "&Сохранить", this,
                        &MainWindow::save_file_clicked, Qt::CTRL | Qt::Key_S);
  file_menu_->addAction("&Выход", this, &MainWindow::close,
                        Qt::CTRL | Qt::Key_Q);

  save_action_ = file_menu_->actions().at(1);
  save_action_->setDisabled(true);
  menu_bar_->addMenu(file_menu_);

  about_menu_ = new QMenu("&О программе", menu_bar_);
  about_menu_->addAction("О программе...", this,
                         &MainWindow::about_menu_clicked);
  menu_bar_->addMenu(about_menu_);
  setMenuBar(menu_bar_);
}

void MainWindow::open_file_clicked() {
  QString file_path = QFileDialog::getOpenFileName(
      this, "Открыть файл", "", "Images (*.bmp *.png *.jpg)");

  if (!file_path.isEmpty()) {
    source_img_widget_->set_image(file_path);
    filter_img_widget_->clear_image();
    filter_img_widget_->set_source_image(file_path);

    if (!save_action_->isEnabled())
      save_action_->setDisabled(false);
  }

  update();
}

void MainWindow::save_file_clicked() {
  QPixmap image = filter_img_widget_->get_blurred_image();
  if (image.isNull())
    return;

  QString filters("BMP (*.bmp);;JPG (*.jpg);;PNG (*.png)");
  QString set_filter;
  QString file_name =
      "Image_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss");
  QString file_path = QFileDialog::getSaveFileName(
      this, "Save picture", file_name, filters, &set_filter);

  bool success = save_image(file_path, set_filter, image);
  if (!success) {
    QMessageBox *error =
        new QMessageBox(QMessageBox::Warning, "Ошибка",
                        "Ошибка сохранения изображения", QMessageBox::Ok, this);
    error->exec();
  }
}

bool MainWindow::save_image(QString file_path, QString set_filter,
                            QPixmap image) {
  bool success = false;
  if (!file_path.isEmpty()) {
    if (set_filter.contains("bmp")) {
      if (!file_path.endsWith(".bmp", Qt::CaseInsensitive))
        file_path = file_path + ".bmp";
      success = image.save(file_path, "BMP");
    } else if (set_filter.contains("jpg")) {
      if (!file_path.endsWith(".jpg", Qt::CaseInsensitive))
        file_path = file_path + ".jpg";
      success = image.save(file_path, "JPG");
    } else if (set_filter.contains("png")) {
      if (!file_path.endsWith(".png", Qt::CaseInsensitive))
        file_path = file_path + ".png";
      success = image.save(file_path, "PNG");
    }
  }

  return success;
}

void MainWindow::about_menu_clicked() {
  QMessageBox *about = new QMessageBox(
      QMessageBox::Information, "О программе",
      "Фильтр размытия по Гауссу\nВерсия 1.0\nРазработчик: Сорокина А.А.",
      QMessageBox::Ok, this);

  about->exec();
}
