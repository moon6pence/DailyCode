import wx
import sys
import re
import threading
import textrank

if hasattr(sys, "setdefaultencoding"): 
	sys.setdefaultencoding('mbcs')

class MyApp(wx.App):
	def OnInit(self):
		frame = MyFrame()
		frame.Show(True)
		self.SetTopWindow(frame)
		wx.STAY_ON_TOP

		return True
	   
class MyStatusBar(wx.StatusBar):
	def __init__(self, parent):
		wx.StatusBar.__init__(self,parent,-1)
		self.SetFieldsCount(1)
		self.SetStatusText('NLPwithPyton Group 3'.decode("utf-8", 'replace').encode("cp949","replace"),0)
	   
class MyFrame(wx.Frame):
	def __init__(self):
		wx.Frame.__init__(self, None, -1, "Text Rank Viewer", size=(1010, 700), style=wx.DEFAULT_FRAME_STYLE|wx.NO_FULL_REPAINT_ON_RESIZE)
		self.sizeChanged = False

		menuFile = wx.Menu()
		menuFile.Append(1, "&About...")
		menuFile.AppendSeparator()
		menuFile.Append(2, "E&xit")
				
		menuBar = wx.MenuBar()
		menuBar.Append(menuFile, "&File")
		self.SetMenuBar(menuBar)
		
		panel = wx.Panel(self, -1)
		panel.SetBackgroundColour(wx.Color(255,255,220))
		
		self.statusBar = MyStatusBar(self)
		self.SetStatusBar(self.statusBar)
		
		self.p_num = wx.TextCtrl(panel, -1, "", pos=(8, 10), size=(40,20))
		wx.StaticText(panel, -1, "URL", pos=(270, 14), size=(25,20))
		self.p_url = wx.TextCtrl(panel, -1, "", pos=(295, 10), size=(690,22))
		self.p_url.SetEditable(False) 
		
		self.go = wx.Button(panel, -1, "Go".decode("utf-8", 'replace').encode("cp949","replace"), pos=(56, 10), size=(63,22))
		self.prev = wx.Button(panel, -1, "Prev".decode("utf-8", 'replace').encode("cp949","replace"), pos=(124, 10), size=(63,22))
		self.next = wx.Button(panel, -1, "Next".decode("utf-8", 'replace').encode("cp949","replace"), pos=(192, 10), size=(63,22))
		
		self.eng = wx.Button(panel, -1, "English".decode("utf-8", 'replace').encode("cp949","replace"), pos=(56, 57), size=(63,22))
		self.kor = wx.Button(panel, -1, "Korean".decode("utf-8", 'replace').encode("cp949","replace"), pos=(124, 57), size=(63,22))
		self.log = wx.Button(panel, -1, "Log".decode("utf-8", 'replace').encode("cp949","replace"), pos=(192, 57), size=(63,22))
		self.graph = wx.Button(panel, -1, "Graph", pos=(260, 57), size=(63,22))
				
		wx.StaticText(panel, -1, "Title", pos=(8, 90), size=(40,20))
		self.p_title = wx.TextCtrl(panel, -1, "", pos=(56, 87), size=(929,22))
		self.p_title.SetEditable(False) 
		wx.StaticText(panel, -1, "Article", pos=(8, 115), size=(40,20))
		self.p_article = wx.TextCtrl(panel, -1, "", pos=(56, 112), size=(929,445),style=wx.TE_MULTILINE)
		self.p_article.SetEditable(False)
		
		wx.StaticText(panel, -1, "TagOrg", pos=(8, 565), size=(40,20))
		self.p_tags_org = wx.TextCtrl(panel, -1, "", pos=(56, 562), size=(929,22))
		self.p_tags_org.SetEditable(False)
		wx.StaticText(panel, -1, "Tag", pos=(8, 590), size=(40,20))		
		self.p_tags = wx.TextCtrl(panel, -1, "", pos=(56, 587), size=(861,22))
		self.p_tags.SetEditable(False)
		self.tag_update = wx.Button(panel, -1, "Update".decode("utf-8", 'replace').encode("cp949","replace"), pos=(922, 587), size=(63,22))
		
		self.Bind(wx.EVT_MENU, self.OnAbout, id=1)
		self.Bind(wx.EVT_MENU, self.OnQuit, id=2)
		
		self.Bind(wx.EVT_BUTTON, self.OnPrev, self.prev)
		self.Bind(wx.EVT_BUTTON, self.OnNext, self.next)
		self.Bind(wx.EVT_BUTTON, self.OnGo, self.go)

		self.Bind(wx.EVT_BUTTON, self.OnEng, self.eng)
		self.Bind(wx.EVT_BUTTON, self.OnKor, self.kor)
		self.Bind(wx.EVT_BUTTON, self.OnLog, self.log)
		self.Bind(wx.EVT_BUTTON, self.OnGraph, self.graph)
		
		self.Bind(wx.EVT_BUTTON, self.OnTag_Update, self.tag_update)
		
		self.num = 0
		self.select = "english"

		self.Update()
		
	def OnQuit(self, event):
		self.Close()
         
	def OnAbout(self, event):
		wx.MessageBox("You can see information about the TextRank".decode("utf-8", 'replace').encode("cp949","replace"), 
			"About...", wx.OK | wx.ICON_INFORMATION, self)
                
	def OnPrev(self, event):
		if self.num == 0:
			self.num = 999
		else:
			self.num -= 1

		self.Update()
			
	def OnNext(self, event):
		if self.num == 999:
			self.num = 0
		else:
			self.num += 1

		self.Update()
	
	def OnGo(self, event):
		if self.p_num.GetValue().isdigit():
			self.num = int(float(self.p_num.GetValue()))
		else:
			self.num = 0

		self.Update()

	def OnEng(self, event):
		self.select = "english"
		self.Update()

	def OnKor(self, event):
		self.select = "korean"
		self.Update()
	
	def OnLog(self, event):
		self.select = "log"
		self.Update()	

	def OnTag_Update(self, event):
		textrank.run(self.num)
		self.Update()
		
	def OnGraph(self, event):
		textrank.run(self.num, show_graph=True)
		self.Update()	

	def Update(self):
		path = "data/%04d/" % self.num
							
		try:
			url_en = open(path+"url_en.txt").read()
		except IOError:
			url_en = ""

		try:
			article_ko = open(path+"article_ko.txt").read()
		except IOError:
			article_ko = ""

		try:
			article_en = open(path+"article_en.txt").read()
		except IOError:
			article_en = ""		

		try:
			log = open(path+"log.txt").read()
		except IOError:
			log = ""	

		try:		
			title = open(path+"title.txt").read()
		except IOError:
			title = ""

		try:
			tags_org = open(path+"tags_org.txt").read()
		except IOError:
			tags_org = ""

		try:
			tags = open(path+"tags.txt").read()
		except IOError:
			tags = ""
		
		self.p_num.SetValue("%04d" % self.num)
		self.p_url.SetValue(url_en.decode("utf-8", 'replace').encode("cp949","replace"))
		self.p_title.SetValue(title.decode("utf-8", 'replace').encode("cp949","replace"))
		self.p_tags_org.SetValue(tags_org.decode("utf-8", 'replace').encode("cp949","replace"))
		self.p_tags.SetValue(tags.decode("utf-8", 'replace').encode("cp949","replace"))	
		
		article_en = self.Beautifulize(article_en)
		article_ko = self.Beautifulize(article_ko)

		if self.select == "english":
			self.p_article.SetValue(article_en.decode("utf-8", 'replace').encode("cp949","replace"))
		elif self.select == "korean":
			self.p_article.SetValue(article_ko.decode("utf-8", 'replace').encode("cp949","replace"))
		elif self.select == "log":
			self.p_article.SetValue(log.decode("utf-8", 'replace').encode("cp949","replace"))
	
	def Beautifulize(self, text):
		patturn = "(\n)+( )*(\n)+( )*((\n)+( )*)*"
		text = re.sub(patturn, "\n ", text)
		text = re.sub(" ( )+", " ", text)
		text = re.sub("\n ", "\n\n ", text)			
		return text

if __name__ == '__main__':
	app = MyApp(False)
	app.MainLoop()
