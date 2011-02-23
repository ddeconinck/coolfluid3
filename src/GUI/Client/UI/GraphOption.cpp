// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

// Qt headers
#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>

// Qwt headers
#include "qwt/qwt_plot_curve.h"

// headers
#include "GUI/Client/Core/NLog.hpp"
#include "GUI/Client/UI/GraphOption.hpp"
#include "GUI/Client/UI/ColorSelector.hpp"
#include "fparser/fparser.hh"

////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace GUI {
namespace ClientUI {

////////////////////////////////////////////////////////////////////////////////


  GraphOption::GraphOption(std::vector< std::vector<double> > & fcts,
                           std::vector<QString> & fcts_label,
                           QwtPlot * ptr_plot,QWidget *parent) :
  //
          QWidget(parent)
  {
      m_ptr_plot = ptr_plot;

      QVBoxLayout * vertical_graph_option_layout = new QVBoxLayout();
      this->setLayout(vertical_graph_option_layout);
      QHBoxLayout * horisontal_table_layout = new QHBoxLayout();
      QVBoxLayout * vertical_line_table_layout = new QVBoxLayout();
      QVBoxLayout * vertical_data_table_layout = new QVBoxLayout();
      QVBoxLayout * vertical_line_button_layout = new QVBoxLayout();
      QHBoxLayout * horisontal_function_name_layout = new QHBoxLayout();
      QHBoxLayout * horisontal_function_function_layout = new QHBoxLayout();

      //creating and initialising the option table with 5 columns
      m_line_table = new QTableWidget(0,5,this);
      //creating and initialising the data table with 2 columns
      m_data_table = new QTableWidget(0,2,this);
      m_data_table->setFixedWidth(250);

      //add line button
      QPushButton * button_add_line = new QPushButton("+");
      QPushButton * button_select_all = new QPushButton("A");
      QPushButton * button_clear_selection = new QPushButton("*");
      QPushButton * button_remove_line = new QPushButton("-");

      //labels of the line_table's columns
      QStringList line_table_labels;
      line_table_labels.push_back(QString(""));
      line_table_labels.push_back(QString("x"));
      line_table_labels.push_back(QString("y"));
      line_table_labels.push_back(QString("Color"));
      line_table_labels.push_back(QString("Line Type"));

      //labels of the data_table's columns
      QStringList data_table_labels;
      data_table_labels.push_back(QString("name"));
      data_table_labels.push_back(QString("function"));

      //seting the columns labels to the table
      m_line_table->setHorizontalHeaderLabels(line_table_labels);
      m_data_table->setHorizontalHeaderLabels(data_table_labels);

      //selection type
      m_line_table->setSelectionMode(QAbstractItemView::MultiSelection);
      m_line_table->setSelectionBehavior(QAbstractItemView::SelectRows);
      m_data_table->setSelectionMode(QAbstractItemView::NoSelection);

      button_draw = new QPushButton("Draw function(s)");

      m_line_function_name = new QLineEdit();
      m_line_function_name->setFixedWidth(180);
      m_line_function = new QLineEdit();
      m_line_function->setFixedWidth(180);

      button_generate_function = new QPushButton("Generate Function");
      button_generate_function->setFixedWidth(250);

      set_data(fcts,fcts_label);

      //adding widget & layout to layout
      vertical_line_table_layout->addWidget(m_line_table);
      vertical_line_table_layout->addWidget(button_draw);

      vertical_data_table_layout->addWidget(m_data_table);

      QLabel * name_fct = new QLabel("Name :");
      name_fct->setFixedWidth(70);
      horisontal_function_name_layout->addWidget(name_fct);
      horisontal_function_name_layout->addWidget(m_line_function_name);

      QLabel * fct_fct = new QLabel("Function :");
      fct_fct->setFixedWidth(70);
      horisontal_function_function_layout->addWidget(fct_fct);
      horisontal_function_function_layout->addWidget(m_line_function);

      vertical_data_table_layout->addLayout(horisontal_function_name_layout);
      vertical_data_table_layout->addLayout(horisontal_function_function_layout);

      vertical_data_table_layout->addWidget(button_generate_function);

      vertical_line_button_layout->addWidget(button_add_line);
      vertical_line_button_layout->addWidget(button_select_all);
      vertical_line_button_layout->addWidget(button_clear_selection);
      vertical_line_button_layout->addWidget(button_remove_line);

      horisontal_table_layout->addLayout(vertical_data_table_layout);
      horisontal_table_layout->addLayout(vertical_line_table_layout);
      horisontal_table_layout->addLayout(vertical_line_button_layout);
      vertical_graph_option_layout->addLayout(horisontal_table_layout);

      connect (button_draw, SIGNAL (clicked()), this, SLOT (draw_and_resize()));
      connect (button_generate_function, SIGNAL (clicked()), this, SLOT (generate_function()));
      connect (button_add_line, SIGNAL (clicked()), this, SLOT (add_line()));
      connect (button_remove_line, SIGNAL (clicked()), this, SLOT (remove_line()));
      connect (button_clear_selection, SIGNAL (clicked()), this, SLOT
               (clear_line_table_selection()));
      connect (button_select_all, SIGNAL (clicked()), this, SLOT
               (select_all_line_table()));
  }


  void GraphOption::draw_action(){

    button_draw->setEnabled(false);

    m_ptr_plot->clear(); //detache all curve and clear the legend

    for(int i = 0; i < m_line_table->rowCount(); ++i ){

      if(((QCheckBox *)m_line_table->cellWidget(i,0))->isChecked()){
        ////new curve
        QwtPlotCurve * new_curve =
            new QwtPlotCurve(((QComboBox *)m_line_table->cellWidget(i,2))->
                             currentText());
        new_curve->setPen(QPen(((ColorSelector *)m_line_table->cellWidget(i,3))->
                               get_color()));
        new_curve->setStyle((QwtPlotCurve::CurveStyle)
                            (((QComboBox *)m_line_table->cellWidget(i,4))->
                             currentIndex()+1));
        new_curve->setData(&m_fcts[((QComboBox *)m_line_table->cellWidget(i,1))->
                                   currentIndex()][0],
                           &m_fcts[((QComboBox *)m_line_table->cellWidget(i,2))->
                                   currentIndex()][0],
                           std::min(m_fcts[((QComboBox *)m_line_table->cellWidget(i,1))->
                                           currentIndex()].size(),
                                    m_fcts[((QComboBox *)m_line_table->cellWidget(i,2))->
                                           currentIndex()].size()));
        new_curve->attach(m_ptr_plot);
      }

    }

    m_ptr_plot->replot();

    button_draw->setEnabled(true);

  }

  void GraphOption::draw_and_resize(){

      draw_action();

      //reinitialise the axis
      m_ptr_plot->setAxisAutoScale(QwtPlot::xBottom);
      m_ptr_plot->setAxisAutoScale(QwtPlot::yLeft);

  }

  void GraphOption::set_data(std::vector< std::vector<double> > & fcts,std::vector<QString> & fcts_label){

    //saving user's function
    std::vector<QString> user_fct_name;
    std::vector<QString> user_fct_fct;

    for(int i=0;i<m_data_table->rowCount();++i)
    {
      if(!(((QLabel *)m_data_table->cellWidget(i,1))->text()).isEmpty())
      {
        user_fct_name.push_back(((QLabel *)m_data_table->cellWidget(i,0))->text());
        user_fct_fct.push_back(((QLabel *)m_data_table->cellWidget(i,1))->text());
      }
    }
    //
/*
    if(fcts_label.size()>0){
      m_data_table->setRowCount(fcts_label.size()+1);//because of the #
      m_data_table->setCellWidget(0,0,new QLabel("#"));
      m_data_table->setCellWidget(0,1,new QLabel());
      m_fcts.clear();
      generate_base_function(fcts[0].size());
      m_fcts.push_back(m_base_function);
      for(int i = 0; i < fcts_label.size(); ++i )
      {
        m_fcts.push_back(fcts[i-1]);
      }
    }else{
    */
      m_fcts = fcts;

      //adding row for eatch data
      m_data_table->setRowCount(fcts_label.size());
    //}

    for(int i = 0; i < m_data_table->rowCount(); ++i )
    {
      m_data_table->setCellWidget(i,0,new QLabel(fcts_label[i]));
      m_data_table->setCellWidget(i,1,new QLabel());
    }

    //regenerate existing user function with new values
    for(int i=0;i<user_fct_fct.size();++i)
    {
        generate_function(user_fct_name[i],user_fct_fct[i]);
    }


    //add #
    if(fcts_label.size()>0){
      m_data_table->setRowCount(m_data_table->rowCount()+1);//because of the #
      m_data_table->setCellWidget(m_data_table->rowCount()-1,0,new QLabel("#"));
      m_data_table->setCellWidget(m_data_table->rowCount()-1,1,new QLabel());

      std::vector<double> vector_temp(fcts[0].size());

      for(int i=0;i<fcts[0].size();++i)
      {
        vector_temp[i] = i;
      }

      m_fcts.push_back(vector_temp);
    }


    //refresh line_table function list
    QStringList function_list;
    for(int i=0;i<m_data_table->rowCount();++i){
      function_list.append(((QLabel *)m_data_table->cellWidget(i,0))->text());
    }

    int current_index;
    for(int i=0;i<m_line_table->rowCount();++i){
      current_index = ((QComboBox *)m_line_table->cellWidget(i,1))->currentIndex();
      ((QComboBox *)m_line_table->cellWidget(i,1))->clear();
      ((QComboBox *)m_line_table->cellWidget(i,1))->addItems(function_list);
      ((QComboBox *)m_line_table->cellWidget(i,1))->setCurrentIndex(current_index);

      current_index = ((QComboBox *)m_line_table->cellWidget(i,2))->currentIndex();
      ((QComboBox *)m_line_table->cellWidget(i,2))->clear();
      ((QComboBox *)m_line_table->cellWidget(i,2))->addItems(function_list);
      ((QComboBox *)m_line_table->cellWidget(i,2))->setCurrentIndex(current_index);
    }

    //draw existing line with new data
    draw_action();

  }


  void GraphOption::add_data(std::vector<double> & fct,QString function_name, QString formula)
  {

    //adding new function to the function set
    m_fcts.push_back(fct);

    //adding one row
    int old_row_count = m_data_table->rowCount();
    m_data_table->setRowCount(1 + old_row_count);

    m_data_table->setCellWidget(old_row_count,0,new QLabel(function_name));
    m_data_table->setCellWidget(old_row_count,1,new QLabel(formula));

    //adding the function to lines x,y lists
    for(int i=0;i<m_line_table->rowCount();++i)
    {
      ((QComboBox *)m_line_table->cellWidget(i,1))->addItem(function_name);
      ((QComboBox *)m_line_table->cellWidget(i,2))->addItem(function_name);
    }
  }

  void  GraphOption::generate_function(){
    generate_function(m_line_function_name->text(),m_line_function->text());
  }

  void  GraphOption::generate_function(QString name,QString formula){

    button_generate_function->setEnabled(false);


    //ferification of name and formula input
    if(name.isEmpty() || formula.isEmpty()){
      ClientCore::NLog::globalLog()->addError("Please give function's name and formula.");
      button_generate_function->setEnabled(true);
      return;
    }

    //check if the function name already exist
    for(int i=0; i < m_fcts.size(); ++i){
      if((((QLabel *)m_data_table->cellWidget(i,0))->text()) == name){
        ClientCore::NLog::globalLog()->addError("The function name already exist.");
        button_generate_function->setEnabled(true);
        return;
      }
    }

    FunctionParser fparser;

    //get all fct name separated by ','
    QString variable = "";
    for(int i=0; i < m_fcts.size(); ++i){
      if(!(((QLabel *)m_data_table->cellWidget(i,0))->text()).isEmpty()){
        if(!variable.isEmpty()){
          variable += ",";
        }
        variable.append(((QLabel *)m_data_table->cellWidget(i,0))->text());
      }
    }

    //parsing function
    int res = fparser.Parse(formula.toStdString().c_str(),
                            variable.toStdString().c_str());

    if(res > 0){
      ClientCore::NLog::globalLog()->addError("The function is not recognized.");
      button_generate_function->setEnabled(true);
      return;
    }

    //<to remove>
    int max_it = 0;
    if(m_fcts.size()>0)
    {
      max_it = m_fcts[0].size();
      for(int i=1;i<m_fcts.size();++i)
      {
        max_it = std::min((int)max_it,(int)m_fcts[i].size());
      }
    }else{
      ClientCore::NLog::globalLog()->addError("The function is not recognized.");
      button_generate_function->setEnabled(true);
      return;
    }
    //</to remove>

    //generate the new function's data
    std::vector<double> vector_temp(max_it);

    double vals[m_fcts.size()];
    for(int i=0;i<max_it;++i)
    {
      for(int j=0;j<m_fcts.size();++j)
      {
        vals[j] = m_fcts[j][i];
      }
      vector_temp[i] = fparser.Eval(vals);
    }

    //add the new function
    this->add_data(vector_temp,name,formula);

    //get back the button and clear the name and function line
    button_generate_function->setEnabled(true);
    m_line_function->clear();
    m_line_function_name->clear();

  }

  void GraphOption::add_line(){

    if(m_data_table->rowCount() <= 0){
      ClientCore::NLog::globalLog()->addError("There are no data to set, you cannot add line");
      return;
    }

      //adding row for eatch function
      int old_row_count = m_line_table->rowCount();
      m_line_table->setRowCount(1 + old_row_count);

      QStringList function_list;
      for(int i=0;i<m_data_table->rowCount();++i){
        function_list.append(((QLabel *)m_data_table->cellWidget(i,0))->text());
      }

      m_line_table->setCellWidget(old_row_count,0,new QCheckBox());
      ((QCheckBox *)m_line_table->cellWidget(old_row_count,0))->setFixedWidth(20);

      m_line_table->setCellWidget(old_row_count,1,new QComboBox());
      ((QComboBox *)m_line_table->cellWidget(old_row_count,1))->addItems(function_list);


      m_line_table->setCellWidget(old_row_count,2,new QComboBox());
      ((QComboBox *)m_line_table->cellWidget(old_row_count,2))->addItems(function_list);


      m_line_table->setCellWidget(old_row_count,3,new ColorSelector());


      m_line_table->setCellWidget(old_row_count,4,new QComboBox());
      ((QComboBox *)m_line_table->cellWidget(old_row_count,4))->addItem("Lines",1);
      ((QComboBox *)m_line_table->cellWidget(old_row_count,4))->addItem("Sticks",2);
      ((QComboBox *)m_line_table->cellWidget(old_row_count,4))->addItem("Steps",3);
      ((QComboBox *)m_line_table->cellWidget(old_row_count,4))->addItem("Dots",4);
      //((QComboBox *)tableau->cellWidget(old_row_count,4))->addItem("dotted line",5);


      /*

    connect(((QCheckBox *)m_line_table->cellWidget(old_row_count,0)),
             SIGNAL(stateChanged(int)), this, SLOT (draw_action()));
    connect(((QComboBox *)m_line_table->cellWidget(old_row_count,1)),
             SIGNAL(currentIndexChanged(int)), this, SLOT (draw_action()));
    connect(((QComboBox *)m_line_table->cellWidget(old_row_count,2)),
             SIGNAL(currentIndexChanged(int)), this, SLOT (draw_action()));
    connect(((ColorSelector *)m_line_table->cellWidget(old_row_count,3)),
             SIGNAL(valueChanged()), this, SLOT (draw_action()));
    connect(((QComboBox *)m_line_table->cellWidget(old_row_count,4)),
             SIGNAL(currentIndexChanged(int)), this, SLOT (draw_action()));
*/

      connect(((QComboBox *)m_line_table->cellWidget(old_row_count,1)),
               SIGNAL(currentIndexChanged(int)), this, SLOT (draw_action()));
      connect(((QComboBox *)m_line_table->cellWidget(old_row_count,2)),
               SIGNAL(currentIndexChanged(int)), this, SLOT (draw_action()));
      connect(((QCheckBox *)m_line_table->cellWidget(old_row_count,0)),
              SIGNAL(stateChanged(int)), this, SLOT (checked_changed(int)));
      connect(((ColorSelector *)m_line_table->cellWidget(old_row_count,3)),
              SIGNAL(valueChanged(QColor)), this, SLOT (color_changed(QColor)));
      connect(((QComboBox *)m_line_table->cellWidget(old_row_count,4)),
              SIGNAL(activated(int)), this, SLOT (line_type_changed(int)));
  }

  void GraphOption::remove_line()
  {
    while( m_line_table->selectionModel()->selectedRows().count() > 0)
    {
      m_line_table->removeRow(m_line_table->selectionModel()->selectedRows().at(0).row());
    }
    draw_action();
  }


  void GraphOption::checked_changed(int check)
  {
    for(int i=0;i<m_line_table->selectionModel()->selectedRows().count();++i)
    {
      ((QCheckBox *)m_line_table->cellWidget(
          m_line_table->selectionModel()->selectedRows().at(i).row(),0))->
          setCheckState((Qt::CheckState)check);
    }
    draw_action();
  }

  void GraphOption::color_changed(QColor color)
  {
    for(int i=0;i<m_line_table->selectionModel()->selectedRows().count();++i)
    {
      ((ColorSelector *)m_line_table->cellWidget(
          m_line_table->selectionModel()->selectedRows().at(i).row(),3))->
          set_color(color);
    }
    draw_action();
  }

  void GraphOption::line_type_changed(int current_index)
  {
    for(int i=0;i<m_line_table->selectionModel()->selectedRows().count();++i)
    {
      ((QComboBox *)m_line_table->cellWidget(
          m_line_table->selectionModel()->selectedRows().at(i).row(),4))->
          setCurrentIndex(current_index);
    }
    draw_action();
  }

  void GraphOption::clear_line_table_selection(){
    m_line_table->clearSelection();
  }

  void GraphOption::select_all_line_table(){
    m_line_table->selectAll();
  }

////////////////////////////////////////////////////////////////////////////////

} // ClientUI
} // GUI
} // CF
