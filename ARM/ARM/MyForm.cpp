#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Data::OleDb;
using namespace System::Net;
using namespace System::Net::Mail;
using namespace System::Net::Mime;
using namespace System::Threading;
using namespace System::ComponentModel;

[STAThread]
void main(array<String^>^ arg)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ARM::MyForm form;
	Application::Run(% form);
}

System::Void ARM::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0;Data Source=Base.mdb";
	OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);

	dbConnection->Open();
	String^ query = "SELECT * FROM [Base]";
	OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);
	OleDbDataReader^ dbReader = dbComand->ExecuteReader();

	if (dbReader->HasRows == false)
	{
		MessageBox::Show("Обшипка считывания данных", "Îøèáêà!");
	}
	else
	{
		while (dbReader->Read())
		{
			dataGridView1->Rows->Add(dbReader["Код"], dbReader["Название"], dbReader["Количество"], dbReader["Дневное изменение"], dbReader["Цена за ед"], dbReader["Прибыль/расход"]);
		}
	}

	dbReader->Close();
	dbConnection->Close();

	return System::Void();
}
System::Void ARM::MyForm::button2_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (dataGridView1->SelectedRows->Count != 1)
	{
		MessageBox::Show("Âûáåðèòå îäíó ñòðîêó äëÿ äîáàâëåíèÿ!", "Âíèìàíèå!");
		return;
	}

	int index = dataGridView1->SelectedRows[0]->Index;

	if (dataGridView1->Rows[index]->Cells[0]->Value == nullptr ||
		dataGridView1->Rows[index]->Cells[1]->Value == nullptr ||
		dataGridView1->Rows[index]->Cells[2]->Value == nullptr ||
		dataGridView1->Rows[index]->Cells[3]->Value == nullptr ||
		dataGridView1->Rows[index]->Cells[4]->Value == nullptr ||
		dataGridView1->Rows[index]->Cells[5]->Value == nullptr) {
		MessageBox::Show("Не все данные введены", "Âíèìàíèå!");
		return;
	}

	String^ id = dataGridView1->Rows[index]->Cells[0]->Value->ToString();
	String^ name = dataGridView1->Rows[index]->Cells[1]->Value->ToString();
	String^ value = dataGridView1->Rows[index]->Cells[2]->Value->ToString();
	String^ day_change = dataGridView1->Rows[index]->Cells[2]->Value->ToString();
	String^ cost = dataGridView1->Rows[index]->Cells[2]->Value->ToString();
	String^ quantity = dataGridView1->Rows[index]->Cells[3]->Value->ToString();

	String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0;Data Source=Base.mdb";
	OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);

	dbConnection->Open();
	String^ query = "INSERT INTO [Base] VALUES (" + id + ", '" + name + "', " + value + ", " + day_change + ", " + cost + ", " + quantity + ")";
	OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);

	if (dbComand->ExecuteNonQuery() != 1)
		MessageBox::Show("Îøèáêà âûïîëíåíèÿ çàïðîñà!", "Îøèáêà!");
	else
		MessageBox::Show("Äàííûå äîáàâëåíû!", "Ãîòîâî!");

	dbConnection->Close();

	return System::Void();
}
System::Void ARM::MyForm::button3_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (dataGridView1->SelectedRows->Count != 1) {
		MessageBox::Show("Âûáåðèòå îäíó ñòðîêó äëÿ äîáàâëåíèÿ!", "Âíèìàíèå!");
		return;
	}

	int index = dataGridView1->SelectedRows[0]->Index;

	if (dataGridView1->Rows[index]->Cells[0]->Value == nullptr)
	{
		MessageBox::Show("Íå âñå äàííûå ââåäåíû!", "Âíèìàíèå!");
		return;
	}

	String^ id = dataGridView1->Rows[index]->Cells[0]->Value->ToString();

	String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0;Data Source=Base.mdb";
	OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);

	dbConnection->Open();
	String^ query = "DELETE FROM [Base] WHERE Код = " + id + "";
	OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);

	if (dbComand->ExecuteNonQuery() != 1)
		MessageBox::Show("Îøèáêà âûïîëíåíèÿ çàïðîñà!", "Îøèáêà!");
	else {
		MessageBox::Show("Äàííûå óäàëåíû!", "Ãîòîâî!");
		dataGridView1->Rows->RemoveAt(index);
	}

	dbConnection->Close();

	return System::Void();
}
System::Void ARM::MyForm::button4_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0;Data Source=Team.mdb";
	OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);

	dbConnection->Open();
	String^ query = "SELECT * FROM [Team]";
	OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);
	OleDbDataReader^ dbReader = dbComand->ExecuteReader();

	if (dbReader->HasRows == false)
	{
		MessageBox::Show("Обшипка считывания данных", "Îøèáêà!");
	}
	else
	{
		while (dbReader->Read())
		{
			dataGridView2->Rows->Add(dbReader["Код"], dbReader["ФИО"], dbReader["График"], dbReader["Зарплата"], dbReader["Статус"], dbReader["Номер договора"]);
		}
	}

	dbReader->Close();
	dbConnection->Close();

	return System::Void();
}
System::Void ARM::MyForm::button5_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (dataGridView2->SelectedRows->Count != 1)
	{
		MessageBox::Show("Âûáåðèòå îäíó ñòðîêó äëÿ äîáàâëåíèÿ!", "Âíèìàíèå!");
		return;
	}

	int index = dataGridView2->SelectedRows[0]->Index;

	if (dataGridView2->Rows[index]->Cells[0]->Value == nullptr ||
		dataGridView2->Rows[index]->Cells[1]->Value == nullptr ||
		dataGridView2->Rows[index]->Cells[2]->Value == nullptr ||
		dataGridView2->Rows[index]->Cells[3]->Value == nullptr ||
		dataGridView2->Rows[index]->Cells[4]->Value == nullptr ||
		dataGridView2->Rows[index]->Cells[5]->Value == nullptr) {
		MessageBox::Show("Не все данные введены", "Âíèìàíèå!");
		return;
	}

	String^ id = dataGridView2->Rows[index]->Cells[0]->Value->ToString();
	String^ name = dataGridView2->Rows[index]->Cells[1]->Value->ToString();
	String^ time = dataGridView2->Rows[index]->Cells[2]->Value->ToString();
	String^ bonus = dataGridView2->Rows[index]->Cells[2]->Value->ToString();
	String^ check = dataGridView2->Rows[index]->Cells[2]->Value->ToString();
	String^ number = dataGridView2->Rows[index]->Cells[3]->Value->ToString();

	String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0;Data Source=Team.mdb";
	OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);

	dbConnection->Open();
	String^ query = "INSERT INTO [Team] VALUES (" + id + ", '" + name + "', '" + time + "', '" + bonus + "', '" + check + "', '" + number + "')";
	OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);

	if (dbComand->ExecuteNonQuery() != 1)
		MessageBox::Show("Îøèáêà âûïîëíåíèÿ çàïðîñà!", "Îøèáêà!");
	else
		MessageBox::Show("Äàííûå äîáàâëåíû!", "Ãîòîâî!");

	dbConnection->Close();

	return System::Void();
}
System::Void ARM::MyForm::button6_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (dataGridView2->SelectedRows->Count != 1) {
		MessageBox::Show("Âûáåðèòå îäíó ñòðîêó äëÿ äîáàâëåíèÿ!", "Âíèìàíèå!");
		return;
	}

	int index = dataGridView2->SelectedRows[0]->Index;

	if (dataGridView2->Rows[index]->Cells[0]->Value == nullptr)
	{
		MessageBox::Show("Íå âñå äàííûå ââåäåíû!", "Âíèìàíèå!");
		return;
	}

	String^ id = dataGridView2->Rows[index]->Cells[0]->Value->ToString();

	String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0;Data Source=Team.mdb";
	OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);

	dbConnection->Open();
	String^ query = "DELETE FROM [Team] WHERE Код = " + id + "";
	OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);

	if (dbComand->ExecuteNonQuery() != 1)
		MessageBox::Show("Îøèáêà âûïîëíåíèÿ çàïðîñà!", "Îøèáêà!");
	else {
		MessageBox::Show("Äàííûå óäàëåíû!", "Ãîòîâî!");
		dataGridView1->Rows->RemoveAt(index);
	}

	dbConnection->Close();

	return System::Void();
}
System::Void ARM::MyForm::button7_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0;Data Source=Equip.mdb";
	OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);

	dbConnection->Open();
	String^ query = "SELECT * FROM [Equip]";
	OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);
	OleDbDataReader^ dbReader = dbComand->ExecuteReader();

	if (dbReader->HasRows == false)
	{
		MessageBox::Show("Обшипка считывания данных", "Îøèáêà!");
	}
	else
	{
		while (dbReader->Read())
		{
			dataGridView3->Rows->Add(dbReader["Код"], dbReader["Оборудование"], dbReader["Статус"], dbReader["Серийный номер"]);
		}
	}

	dbReader->Close();
	dbConnection->Close();

	return System::Void();
}
System::Void ARM::MyForm::button8_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (dataGridView3->SelectedRows->Count != 1)
	{
		MessageBox::Show("Âûáåðèòå îäíó ñòðîêó äëÿ äîáàâëåíèÿ!", "Âíèìàíèå!");
		return;
	}

	int index = dataGridView3->SelectedRows[0]->Index;

	if (dataGridView3->Rows[index]->Cells[0]->Value == nullptr ||
		dataGridView3->Rows[index]->Cells[1]->Value == nullptr ||
		dataGridView3->Rows[index]->Cells[2]->Value == nullptr ||
		dataGridView3->Rows[index]->Cells[3]->Value == nullptr) {
		MessageBox::Show("Не все данные введены", "Âíèìàíèå!");
		return;
	}

	String^ id = dataGridView3->Rows[index]->Cells[0]->Value->ToString();
	String^ name = dataGridView3->Rows[index]->Cells[1]->Value->ToString();
	String^ check = dataGridView3->Rows[index]->Cells[2]->Value->ToString();
	String^ number = dataGridView3->Rows[index]->Cells[2]->Value->ToString();

	String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0;Data Source=Equip.mdb";
	OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);

	dbConnection->Open();
	String^ query = "INSERT INTO [Equip] VALUES (" + id + ", '" + name + "', '" + check + "', '" + number + "')";
	OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);

	if (dbComand->ExecuteNonQuery() != 1)
		MessageBox::Show("Îøèáêà âûïîëíåíèÿ çàïðîñà!", "Îøèáêà!");
	else
		MessageBox::Show("Äàííûå äîáàâëåíû!", "Ãîòîâî!");

	dbConnection->Close();

	return System::Void();
}
System::Void ARM::MyForm::button9_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (dataGridView3->SelectedRows->Count != 1) {
		MessageBox::Show("Âûáåðèòå îäíó ñòðîêó äëÿ äîáàâëåíèÿ!", "Âíèìàíèå!");
		return;
	}

	int index = dataGridView3->SelectedRows[0]->Index;

	if (dataGridView3->Rows[index]->Cells[0]->Value == nullptr)
	{
		MessageBox::Show("Íå âñå äàííûå ââåäåíû!", "Âíèìàíèå!");
		return;
	}

	String^ id = dataGridView3->Rows[index]->Cells[0]->Value->ToString();

	String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0;Data Source=Equip.mdb";
	OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);

	dbConnection->Open();
	String^ query = "DELETE FROM [Equip] WHERE Код = " + id + "";
	OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);

	if (dbComand->ExecuteNonQuery() != 1)
		MessageBox::Show("Îøèáêà âûïîëíåíèÿ çàïðîñà!", "Îøèáêà!");
	else {
		MessageBox::Show("Äàííûå óäàëåíû!", "Ãîòîâî!");
		dataGridView3->Rows->RemoveAt(index);
	}

	dbConnection->Close();

	return System::Void();
}
System::Void ARM::MyForm::button10_Click(System::Object^ sender, System::EventArgs^ e)
{
	SmtpClient^ Smtp = gcnew SmtpClient("smtp.mail.ru", 25);
	Smtp->Credentials = gcnew NetworkCredential("nneozen@mail.ru", "BHVG8TTRpU4d56BQK3Sj");
	Smtp->EnableSsl = true;

	MailMessage^ Message = gcnew MailMessage();
	Message->From = gcnew MailAddress("nneozen@mail.ru");
	Message->To->Add(gcnew MailAddress(textBox1->Text));
	Message->Subject = textBox2->Text;
	Message->Body = textBox3->Text;

	Smtp->Send(Message);

	return System::Void();
}
