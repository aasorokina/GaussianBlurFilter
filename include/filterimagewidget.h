#ifndef FILTERIMAGEWIDGET_H
#define FILTERIMAGEWIDGET_H

#include <QtWidgets>

#include "gaussianfilter.h"

class FilterImageWidget : public QWidget {
  Q_OBJECT

public:
  enum FilterRange { MIN = 2, MAX = 20 };

  explicit FilterImageWidget(QWidget *parent = nullptr);
  ~FilterImageWidget();
  void set_image(QPixmap image);
  void set_source_image(const QString &filepath);
  void clear_image();
  int get_blur_radius();
  QPixmap get_blurred_image();
  void setup_controls();
  void setup_progress_dialog();

private:
  QLabel *img_lbl_;
  QLabel *text_lbl_;
  QLabel *curr_value_lbl_;
  QPixmap blurred_image_;
  QSpinBox *filter_spb_;
  QPushButton *apply_filter_btn_;
  GaussianFilter *filter_;
  QSettings settings_;
  QThread filter_thread_;
  QProgressDialog *progress_dialog_;

private slots:
  void apply_botton_clicked();
  void on_blur_completed();
  void update_progress(int percent);
  void cancel_blur();

signals:
  void apply_filter(int value);
};

#endif // FILTERIMAGEWIDGET_H
