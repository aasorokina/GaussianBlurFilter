#include "../include/sourceimgwidget.h"

SourceImgWidget::SourceImgWidget(QWidget *parent) : QWidget(parent) {
  file_name_ = new QLineEdit(this);
  file_name_->setReadOnly(true);

  img_lbl_ = new QLabel(this);
  img_lbl_->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  QScrollArea *scrl_area = new QScrollArea(this);
  scrl_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scrl_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scrl_area->setWidget(img_lbl_);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(file_name_);
  layout->addWidget(scrl_area);
  setLayout(layout);
}

void SourceImgWidget::set_image(const QString &filename) {
  QPixmap image(filename);
  img_lbl_->clear();

  if (!image.isNull()) {
    img_lbl_->setPixmap(image);
  } else {
    img_lbl_->setText(" Невозможно загрузить изображение");
  }

  img_lbl_->adjustSize();
  file_name_->setText(filename);
  update();
}
