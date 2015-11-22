#!/usr/bin/env python
import wx
import os
from codegenerator import *

defaultDirectoy = "C:/program1/FlipEngine1/Engine"
codegen = codegenerator()

def _write_code(impl_file, template, search):
    code = Template(file=os.path.join(templatepath, "templates", template),
        searchList=[search])
    impl_file.write(str(code))

class DirTreeCtrl(wx.TreeCtrl):
	def __init__(self, parent):
		wx.TreeCtrl.__init__(self, parent)

	def setSearchDir(self, directory):
		self.root = self.AddRoot(defaultDirectoy)
		self.__searchDir(self.root, defaultDirectoy)
		# self.ExpandAll()

	def __searchDir(self, item, directory):
		if not os.path.isdir(directory):
			print("is not dir")
			return

		files = os.listdir(directory)
		for f in files:
			file_path = os.path.join(directory, f)
			if os.path.isdir(file_path):
				child_item = self.AppendItem(item, f)
				self.__searchDir(child_item, file_path)
			elif file_path.endswith('.h'):
				self.AppendItem(item, f)

	def getPath(self, item):
		if item == self.root:
			return self.GetItemText(item)
		else:
			parent = self.GetItemParent(item)
			return os.path.join(self.getPath(parent), self.GetItemText(item))
		

class MyFrame(wx.Frame):
	def __init__(self):
		wx.Frame.__init__(self, None, -1, "codetool", size = (800, 600))
		
		panel1 = wx.Panel(self, -1)
		panel2 = wx.Panel(self, -1)

		self.headerbrower = DirTreeCtrl(panel1)
		self.headerbrower.setSearchDir(defaultDirectoy)

		text1 = wx.StaticText(panel2, -1, "classes", style=wx.ALIGN_CENTRE)
		text2 = wx.StaticText(panel2, -1, "methods", style=wx.ALIGN_CENTRE)
		self.classbrower = wx.ListBox(panel2)
		self.methodbrower = wx.CheckListBox(panel2)
		self.genButton = wx.Button(panel2, -1, 'Generator Code')

		hbox = wx.BoxSizer(wx.HORIZONTAL)
		hbox.Add(panel1, 1, wx.EXPAND)
		hbox.Add(panel2, 1, wx.EXPAND)

		vbox1 = wx.BoxSizer(wx.VERTICAL)
		vbox1.Add(self.headerbrower, 1, wx.EXPAND)

		vbox2 = wx.BoxSizer(wx.VERTICAL)
		vbox2.Add(text1, 0, wx.EXPAND)
		vbox2.Add(self.classbrower, 2, wx.EXPAND)
		vbox2.Add(text2, 0, wx.EXPAND)
		vbox2.Add(self.methodbrower, 6, wx.EXPAND)
		vbox2.Add(self.genButton, 1, wx.EXPAND)

		panel1.SetSizer(vbox1)
		panel2.SetSizer(vbox2)
		self.SetSizer(hbox)
		self.Centre()

		self.headerbrower.Bind(wx.EVT_TREE_SEL_CHANGED, self.OnSelChanged)
		self.genButton.Bind(wx.EVT_BUTTON, self.OnBtnGen)
		self.classbrower.Bind(wx.EVT_LISTBOX, self.OnClassSelect)
		self.methodbrower.Bind(wx.EVT_CHECKLISTBOX, self.OnCheckBoxList)
	
	def OnClassSelect(self, event):
  		index = event.GetSelection()
  		# print self.tmpClasses
  		self.selectclass = self.tmpClasses[index]
  		self.methodbrower.Clear()
		for (name, impl) in self.selectclass.methods.items():
			self.methodbrower.Append(impl.ret_type.name +" " +impl.displayname)
			self.tmpMethods.append(impl)

	def OnCheckBoxList(self, event):
		index = event.GetInt()
		check = self.methodbrower.IsChecked(index)
		self.tmpMethods[index].export = check

	def OnSelChanged(self, event):
		self.classbrower.Clear()
		self.methodbrower.Clear()

		item =  event.GetItem()
		file_path = self.headerbrower.getPath(item)
		self.relpath = os.path.relpath(file_path, defaultDirectoy)

		codegen._parse_header(file_path)

		self.tmpMethods = []
		self.tmpClasses = codegen.tmpclass
		for nclass in codegen.tmpclass:
			self.classbrower.Append(nclass.class_name)

	def OnBtnGen(self, event):
		if self.selectclass:
			search = {
		        "header": self.relpath,
		    }
			nclass = self.selectclass
			implfilepath = os.path.join(defaultDirectoy, "autolua", "l" + nclass.class_name + ".cpp")
			impl_file = open(implfilepath, "w+")
			_write_code(impl_file, "layout_head.c", search)
			nclass.generate_code(impl_file)
			# _write_code(impl_file, "layout_foot.c", search)
			impl_file.close()
			print "generate_code finish!!!"

app = wx.App()
frame = MyFrame()  
frame.Show()  
app.MainLoop() 
