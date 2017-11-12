 ///
 /// @file    FileProcessor.cc
 /// @author  yangwenhao
 /// @date    2017-05-27 16:55:50
 ///
 
#include "FileProcessor.h"
#include "TaskUtil.h"

#include <boost/regex.hpp>

#include <iostream>
using std::cout;
using std::endl;


namespace sh
{

FileProcessor::FileProcessor()
{
}

vector<string> & FileProcessor::process(int & idx, const string & filename)
{
	XMLDocument doc;
	doc.LoadFile(filename.c_str());
	XMLError errorId = doc.ErrorID();
	if(errorId)
	{
		cout << "FileProcessor: load file error!" << endl;
		exit(EXIT_FAILURE);
	}

	XMLElement * root = doc.FirstChildElement();
	XMLElement * channel = root->FirstChildElement("channel");
	XMLElement * entry = root->FirstChildElement("entry");

	XMLElement * firstChild = NULL;
	if(channel)
	{
		XMLElement * item = channel->FirstChildElement("item");
		firstChild = item;
	}
	else if(entry)
	{
		firstChild = entry;
	}
	else
	{
		cout << "FileProcessor: findFirstChild error!" << endl;
		exit(EXIT_FAILURE);
	}

	for(; firstChild; firstChild = firstChild->NextSiblingElement())
	{
		readDoc(idx, firstChild);
	}

	if(!_vecFiles.empty())
		return _vecFiles;
	else
	{
		cout << "FileProcessor: _vecFiles is empty!" << endl;
		exit(EXIT_FAILURE);
	}
}

void FileProcessor::readDoc(int & idx, XMLElement * firstChild)
{
	Webpage webpage;
	webpage._docid = convert2string(idx);
	
	XMLElement * title = firstChild->FirstChildElement("title");

	XMLElement * id = firstChild->FirstChildElement("id");
	XMLElement * link = firstChild->FirstChildElement("link");
	XMLElement * titleLink = firstChild->FirstChildElement("title")->FirstChildElement("link");

	XMLElement * description = firstChild->FirstChildElement("description");
	XMLElement * contentEncoded = firstChild->FirstChildElement("content:encoded");
	XMLElement * content = firstChild->FirstChildElement("content");
	
	const char * linkPtr = NULL;
	if(id)
		linkPtr = id->GetText();
	else
	{
		if(link)
			linkPtr = link->GetText();
		else if(titleLink)
			linkPtr = titleLink->GetText();
		else
		{
			cout << "FileProcessor: find URL error!" << endl;
			exit(EXIT_FAILURE);
		}
	}
	if(linkPtr)
	{
		webpage._link = linkPtr;
	}

	XMLElement * Content = NULL;
	if(contentEncoded)
		Content = contentEncoded;
	else if(content)
		Content = content;
	else if(description)
		Content = description;
	else
	{
		cout << "FileProcessor: find content error!" << endl;
		exit(EXIT_FAILURE);
	}
	string tmp;
	XMLNode * CDATA = Content->FirstChild();
	for(; CDATA; CDATA = CDATA->NextSibling())
	{
		XMLText * textNode = CDATA->ToText();
		const char * data = textNode->Value();
		tmp.append(data);
	}
	boost::regex reg("<.*?>");
	webpage._content = boost::regex_replace(tmp, reg, string(""));

	const char * titlePtr = NULL;
	if(title)
	{
		titlePtr = title->GetText();
		if(titlePtr)
			webpage._title = titlePtr;
	}
	else
	{
		size_t pos  = webpage._content.find("\n");
		if(pos)
		{
			string tmp = webpage._content.substr(0, pos);
			webpage._title = tmp;
		}
	}

#if 0
	cout << webpage._docid << " " << webpage._title << endl
		 << webpage._link << endl
		 << webpage._content << endl;
#endif
	
	if(!webpage._link.empty())
	{
		string doc;
		doc.append("<doc>\n\t<docid>")
			  .append(webpage._docid).append("</docid>\n\t<title>")
			  .append(webpage._title).append("</title>\n\t<link>")
			  .append(webpage._link).append("</link>\n\t<content>")
			  .append(webpage._content).append("</content>\n</doc>\n");
		_vecFiles.push_back(doc);
		++idx;
	}
}

}// end of namespace sh
