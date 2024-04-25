from tkinter import *
from tkinter import ttk


class ParamField:
    def __init__(self, root, name='Placeholder', variable=None, entry_state='normal'):
        self.elmnt = ttk.Frame(root)
        if variable is None:
            self.inputVar = StringVar()
        else:
            self.inputVar = variable

        self.elmnt.columnconfigure(0, weight=1)
        self.elmnt.columnconfigure(1, weight=0)
        label = ttk.Label(self.elmnt, text=name, justify=LEFT, anchor='w')
        label.grid(row=0, column=0, sticky='EW')

        input_entry = Entry(self.elmnt, width=14, justify='center', textvariable=self.inputVar, state=entry_state)
        input_entry.grid(row=0, column=1, sticky='EW')


class InputSlider:
    def __init__(self, root, limits=(0, 100)):
        self.elmnt = ttk.Frame(root)
        self.elmnt.columnconfigure(0, weight=1)
        self.limits = limits
        self.varString = StringVar()
        self.varFloat = DoubleVar()

        slider_frame = Frame(self.elmnt)
        slider_frame.grid(row=0, column=0)
        slider_frame.columnconfigure(1, weight=1)

        self.slider = ttk.Scale(slider_frame, command=self.__slider_moved__, variable=self.varFloat,
                                from_=limits[0], to=limits[1])
        self.slider.grid(row=0, column=0, columnspan=3, sticky='EW')
        lower_limit_label = Label(slider_frame, text=str(limits[0]), justify=LEFT)
        lower_limit_label.grid(row=1, column=0)
        higher_limit_label = Label(slider_frame, text=str(limits[1]), justify=RIGHT)
        higher_limit_label.grid(row=1, column=2)

        self.input_box = Entry(self.elmnt, justify='center', textvariable=self.varString)
        self.input_box.grid(row=0, column=1, sticky='NSEW')

    def __slider_moved__(self, val):
        self.varString.set("%.2f" % self.varFloat.get())
        pass

    def __input_editted__(self):
        print('edited')
        pass


class TwoStateBtn:
    def __init__(self, root, texts=('Off', 'On'), callbacks=(None, None), default_state=0):
        self.textVar = StringVar()
        self.textVar.set(texts[default_state])
        self.state = default_state
        self.btn_texts = texts
        self.callbacks = callbacks

        self.elmnt = Button(root, textvariable=self.textVar, command=self.__onclick__)

    def __onclick__(self):
        self.state ^= 1
        self.textVar.set(self.btn_texts[self.state])
        if self.callbacks[self.state] is not None:
            self.callbacks[self.state]()
