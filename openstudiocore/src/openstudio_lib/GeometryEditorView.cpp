/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "GeometryEditorView.hpp"
#include "GeometryPreviewView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ThreeJS.hpp"
#include "../model/PlanarSurfaceGroup.hpp"
#include "../model/PlanarSurfaceGroup_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Checksum.hpp"
#include "../utilities/geometry/FloorplanJS.hpp"
#include "../utilities/geometry/ThreeJS.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <QDialog>
#include <QTimer>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWebEnginePage>
#include <QWebEngineSettings>

namespace openstudio {

GeometryEditorView::GeometryEditorView(bool isIP,
                                     const openstudio::model::Model& model,
                                     QWidget * parent)
: QWidget(parent)
{
  // TODO: DLM impliment units switching
  //connect(this, &GeometryEditorView::toggleUnitsClicked, modelObjectInspectorView(), &ModelObjectInspectorView::toggleUnitsClicked);

  QVBoxLayout *layout = new QVBoxLayout;
  
  EditorWebView* webView = new EditorWebView(model, this);
  layout->addWidget(webView);

  setLayout(layout);
}

GeometryEditorView::~GeometryEditorView()
{

}

EditorWebView::EditorWebView(const openstudio::model::Model& model, QWidget *t_parent)
  : QWidget(t_parent),
    m_model(model),
    m_isIP(true),
    m_previewBtn(new QPushButton("Preview")),
    m_progressBar(new QProgressBar()),
    m_refreshBtn(new QPushButton("Refresh")),
    m_mergeBtn(new QPushButton("Merge"))
{

  auto mainLayout = new QVBoxLayout;
  setLayout(mainLayout);

  connect(m_refreshBtn, &QPushButton::clicked, this, &EditorWebView::refreshClicked);
  connect(m_previewBtn, &QPushButton::clicked, this, &EditorWebView::previewClicked);
  connect(m_mergeBtn, &QPushButton::clicked, this, &EditorWebView::mergeClicked);

  auto hLayout = new QHBoxLayout(this);
  mainLayout->addLayout(hLayout);

  hLayout->addStretch();

  hLayout->addWidget(m_progressBar, 0, Qt::AlignVCenter);
  m_progressBar->setMinimum(0);
  m_progressBar->setMaximum(100);
  m_progressBar->setValue(0);
  m_progressBar->setVisible(false); // make visible when load first page

  hLayout->addWidget(m_refreshBtn, 0, Qt::AlignVCenter);
  m_refreshBtn->setVisible(true);

  hLayout->addWidget(m_previewBtn, 0, Qt::AlignVCenter);
  m_previewBtn->setVisible(true);

  hLayout->addWidget(m_mergeBtn, 0, Qt::AlignVCenter);
  m_mergeBtn->setVisible(true);

  m_view = new QWebEngineView(this);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::LocalContentCanAccessRemoteUrls, true);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::SpatialNavigationEnabled, true);

  connect(m_view, &QWebEngineView::loadFinished, this, &EditorWebView::onLoadFinished);
  connect(m_view, &QWebEngineView::loadProgress, this, &EditorWebView::onLoadProgress);
  connect(m_view, &QWebEngineView::loadStarted, this, &EditorWebView::onLoadStarted);
  connect(m_view, &QWebEngineView::renderProcessTerminated, this, &EditorWebView::onRenderProcessTerminated);
  
  // Qt 5.8 and higher
  //m_view->setAttribute(QWebEngineSettings::WebAttribute::AllowRunningInsecureContent, true);

  //m_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_view->setContextMenuPolicy(Qt::NoContextMenu);

  //mainLayout->addWidget(m_view, 10, Qt::AlignTop);
  mainLayout->addWidget(m_view);

  openstudio::path p = floorplanPath();
  if (exists(p)){
    openstudio::filesystem::ifstream ifs(p);
    OS_ASSERT(ifs.is_open());
    std::string contents( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
    ifs.close();
    m_floorplan = FloorplanJS::load(contents);
  }

  m_view->load(QUrl("qrc:///library/geometry_editor.html"));
}

EditorWebView::~EditorWebView()
{
  saveExport();
  delete m_view;
}

void EditorWebView::refreshClicked()
{
  m_view->triggerPageAction(QWebEnginePage::ReloadAndBypassCache);
}

void EditorWebView::previewClicked()
{
  m_previewBtn->setEnabled(false);

  QString javascript = QString("JSON.stringify(api.doExport());");
  m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_export = v; QTimer::singleShot(0, this, &EditorWebView::previewExport);});
}

void EditorWebView::mergeClicked()
{
  m_mergeBtn->setEnabled(false);

  QString javascript = QString("JSON.stringify(api.doExport());");
  m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_export = v; QTimer::singleShot(0, this, &EditorWebView::mergeExport);});
}

void EditorWebView::translateExport()
{
  std::string json = m_export.value<QString>().toStdString();

  boost::optional<model::Model> model;
  boost::optional<FloorplanJS> floorplan = FloorplanJS::load(json);
  if (floorplan){
    ThreeScene scene = floorplan->toThreeScene(true);
    model = model::modelFromThreeJS(scene);
  }
  
  if (model){
    m_exportModel = *model;
  } else{
    //m_exportModel = model::Model();
    m_exportModel = model::exampleModel();
  }
  
}

void EditorWebView::saveExport()
{
  if (!m_export.isNull()){

    std::string contents = m_export.value<QString>().toStdString();

    // DLM: should we compare checksums and only 
    openstudio::path out = floorplanPath();
    if (!out.empty()){
      if (checksum(contents) != checksum(out)){

        m_floorplan = FloorplanJS::load(contents);

        openstudio::filesystem::ofstream file(out);
        OS_ASSERT(file.is_open());
        file << contents;
        file.close();

        openstudio::OSAppBase * app = OSAppBase::instance();
        if (app && app->currentDocument()) {
          app->currentDocument()->markAsModified();
        }
      }
    }
  }
}

void EditorWebView::previewExport()
{
  saveExport();
  translateExport();

  std::stringstream ss;
  ss << m_exportModel;
  std::string s = ss.str();

  PreviewWebView* webView = new PreviewWebView(m_exportModel);
  QLayout* layout = new QVBoxLayout();
  layout->addWidget(webView);

  QDialog* dialog = new QDialog(this, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
  dialog->setModal(true);
  dialog->setWindowTitle("Geometry Preview");
  dialog->setLayout(layout);
  dialog->open();

  m_previewBtn->setEnabled(true);
}

void EditorWebView::mergeExport()
{
  saveExport();
  translateExport();

  // mega lame merge
  for (auto& surfaceGroup : m_model.getModelObjects<model::PlanarSurfaceGroup>()){
    surfaceGroup.remove();
  }
  for (auto& surfaceGroup : m_exportModel.getModelObjects<model::PlanarSurfaceGroup>()){
    surfaceGroup.clone(m_model);
  }

  m_mergeBtn->setEnabled(true);
}

void EditorWebView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_isIP = t_isIP;
}

void EditorWebView::onLoadFinished(bool ok)
{
  if (m_floorplan){
    std::string json = m_floorplan->toJSON(false);
    QString javascript = QString("api.doImport(JSON.stringify(") + QString::fromStdString(json) + QString("));");
    m_view->page()->runJavaScript(javascript);
  }

  QString title = m_view->title();
  if (ok){
    m_progressBar->setStyleSheet("");
    m_progressBar->setFormat("");
    m_progressBar->setTextVisible(false);
  } else{
    m_progressBar->setStyleSheet("QProgressBar::chunk {background-color: #FF0000;}");
    m_progressBar->setFormat("Error");
    m_progressBar->setTextVisible(true);
  }
}

void EditorWebView::onLoadProgress(int progress)
{
  m_progressBar->setStyleSheet("");
  m_progressBar->setFormat("");
  m_progressBar->setTextVisible(false);
  m_progressBar->setValue(progress);
}

void EditorWebView::onLoadStarted()
{
  m_progressBar->setStyleSheet("");
  m_progressBar->setFormat("");
  m_progressBar->setTextVisible(false);
}

void EditorWebView::onRenderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode)
{
  m_progressBar->setStyleSheet("QProgressBar::chunk {background-color: #FF0000;}");
  m_progressBar->setFormat("Error");
  m_progressBar->setTextVisible(true);
}

openstudio::path EditorWebView::floorplanPath() const
{
  openstudio::OSAppBase * app = OSAppBase::instance();
  if (app && app->currentDocument()) {
    return toPath(app->currentDocument()->modelTempDir()) / toPath("resources/floorplan.json");
  }
  return path();
}

} // openstudio
