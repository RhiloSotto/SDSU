import wx

def btnclick(evt):
    print(txt.Value)

theApp = wx.App()
f = wx.Frame(parent = None, title = "Hi")

txt = wx.TextCtrl(parent = f)
btn1 = wx.Button(parent = f, label="display")


#txt.SetPosition(wx.Point(10,20))
#btn1.SetPosition(wx.Point(10, 50))

btn1.Bind(wx.EVT_BUTTON, btnclick)

f.Show()
theApp.MainLoop()
