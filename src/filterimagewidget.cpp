#include "../include/filterimagewidget.h"

FilterImageWidget::FilterImageWidget(QWidget *parent)
    : QWidget(parent), settings_("asorokina", "gaussian_blur") {
  img_lbl_ = new QLabel(this);
  blurred_image_ = QPixmap();
  filter_ = new GaussianFilter();

  setup_progress_dialog();

  connect(this, &FilterImageWidget::apply_filter, filter_,
          &GaussianFilter::set_blur);
  connect(filter_, &GaussianFilter::blur_completed, this,
          &FilterImageWidget::on_blur_completed);
  connect(filter_, &GaussianFilter::progress_updated, this,
          &FilterImageWidget::update_progress);
  connect(progress_dialog_, &QProgressDialog::canceled, this,
          &FilterImageWidget::cancel_blur);

  filter_->moveToThread(&filter_thread_);
  filter_thread_.start();

  setup_controls();
}

void FilterImageWidget::setup_controls() {
  text_lbl_ = new QLabel("Радиус размытия: ", this);
  curr_value_lbl_ = new QLabel("Текущее значение: ", this);

  filter_spb_ = new QSpinBox(this);
  filter_spb_->setRange(FilterRange::MIN, FilterRange::MAX);
  filter_spb_->setButtonSymbols(QSpinBox::PlusMinus);
  filter_spb_->setValue(
      settings_.value("blur_radius", FilterRange::MIN).toInt());

  apply_filter_btn_ = new QPushButton("Применить", this);
  connect(apply_filter_btn_, &QPushButton::clicked, this,
          &FilterImageWidget::apply_botton_clicked);

  QHBoxLayout *h_layout = new QHBoxLayout();
  h_layout->setAlignment(Qt::AlignLeft);
  h_layout->addWidget(text_lbl_);
  h_layout->addWidget(filter_spb_);
  h_layout->addWidget(apply_filter_btn_);
  h_layout->addWidget(curr_value_lbl_);

  QScrollArea *scrl_area = new QScrollArea(this);
  scrl_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scrl_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scrl_area->setWidget(img_lbl_);

  QVBoxLayout *v_layout = new QVBoxLayout(this);
  v_layout->addLayout(h_layout);
  v_layout->addWidget(scrl_area);

  setLayout(v_layout);
}

void FilterImageWidget::setup_progress_dialog() {
  progress_dialog_ =
      new QProgressDialog("Применение фильтра...", "Отмена", 0, 100, this);
  progress_dialog_->setWindowModality(Qt::WindowModal);
  progress_dialog_->setAutoClose(true);
  progress_dialog_->setAutoReset(true);
  progress_dialog_->setMinimumDuration(500);
  progress_dialog_->setWindowTitle(" ");
  progress_dialog_->reset();
}

FilterImageWidget::~FilterImageWidget() {
  settings_.setValue("blur_radius", filter_->get_blur_radius());

  filter_thread_.quit();
  filter_thread_.wait();

  if (filter_ != nullptr) {
    delete filter_;
  }
}

void FilterImageWidget::set_image(QPixmap image) {
  if (!image.isNull()) {
    img_lbl_->setPixmap(image);
    img_lbl_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    img_lbl_->adjustSize();
  }
  update();
}

void FilterImageWidget::set_source_image(const QString &filepath) {
  bool error = filter_->set_source_image(filepath);
  if (error) {
    img_lbl_->setText(" Невозможно загрузить изображение");
    img_lbl_->adjustSize();
  }
}

void FilterImageWidget::clear_image() { img_lbl_->clear(); }

int FilterImageWidget::get_blur_radius() { return filter_->get_blur_radius(); }

QPixmap FilterImageWidget::get_blurred_image() { return blurred_image_; }

void FilterImageWidget::apply_botton_clicked() {
  emit apply_filter(filter_spb_->value());
}

void FilterImageWidget::on_blur_completed() {
  blurred_image_ = filter_->get_blurred_image();
  progress_dialog_->reset();

  if (!blurred_image_.isNull())
    set_image(blurred_image_);
  else {
    img_lbl_->setText(" Ошибка при применении фильтра");
    img_lbl_->adjustSize();
  }

  QLocale locale;
  curr_value_lbl_->setText("Текущее значение: " +
                           locale.toString(filter_spb_->value()));
}

void FilterImageWidget::update_progress(int percent) {
  progress_dialog_->setValue(percent);
}

void FilterImageWidget::cancel_blur() { filter_->cancel_blur(); }
