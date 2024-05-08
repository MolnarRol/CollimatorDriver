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


class DoubleEntry:
    def __init__(self, root, value_changed_callback=None):
        self.string_val = StringVar()
        self.string_val.trace("w", lambda name, index, mode,
                                          var=self.string_val: self.__str_value_changed__(var.get()))
        self.callback = value_changed_callback

        self.elmnt = Entry(root, justify='center', textvariable=self.string_val)
        pass

    def __str_value_changed__(self, var):
        try:
            double_out = float(var)
            if self.callback is not None:
                self.callback(double_out)
        except ValueError:
            pass
        pass

    def set_value(self, val: float):
        self.string_val.set("%.2f" % val)


class InputSlider:
    def __init__(self, root, limits=(0, 100), callback=None):
        self.elmnt = ttk.Frame(root)
        self.elmnt.columnconfigure(0, weight=1)
        self.elmnt.rowconfigure(0, weight=1)
        self.limits = limits
        self.varString = StringVar()
        self.varFloat = DoubleVar()
        self.callback = callback

        slider_frame = Frame(self.elmnt)
        slider_frame.grid(row=0, column=0, sticky='NSEW')
        slider_frame.columnconfigure(1, weight=1)

        self.slider = ttk.Scale(slider_frame, command=self.__slider_moved__, variable=self.varFloat,
                                from_=limits[0], to=limits[1])
        self.slider.grid(row=0, column=0, columnspan=3, sticky='NSEW')
        self.lower_limit_label = Label(slider_frame, text=str(limits[0]), justify=LEFT)
        self.lower_limit_label.grid(row=1, column=0)
        self.higher_limit_label = Label(slider_frame, text=str(limits[1]), justify=RIGHT)
        self.higher_limit_label.grid(row=1, column=2)

        self.input_box = DoubleEntry(self.elmnt, self.__input_edited__)
        self.input_box.elmnt.configure(width=8)
        self.input_box.elmnt.configure(font='Verdana 14')
        self.input_box.elmnt.grid(row=0, column=1, sticky='NSEW')

    def __slider_moved__(self, val):
        self.input_box.set_value(self.varFloat.get())
        if self.callback is not None:
            self.callback(self.varFloat.get())

    def __input_edited__(self, val):
        if self.limits[0] <= val <= self.limits[1]:
            self.varFloat.set(val)
        if self.callback is not None:
            self.callback(self.varFloat.get())

    def update_limits(self, limits=(0, 100)):
        self.limits = limits
        self.lower_limit_label.config(text=str(limits[0]))
        self.higher_limit_label.config(text=str(limits[1]))


class TwoStateBtn:
    def __init__(self, root, texts=('Off', 'On'), callbacks=(None, None), default_state=0):
        self.textVar = StringVar()
        self.textVar.set(texts[default_state])
        self.state = default_state
        self.btn_texts = texts
        self.callbacks = callbacks

        self.elmnt = Button(root, textvariable=self.textVar, command=self.__onclick__, padx=10)

    def __onclick__(self):
        self.state ^= 1
        self.textVar.set(self.btn_texts[self.state])
        if self.callbacks[self.state] is not None:
            self.callbacks[self.state]()

    def overwrite_state(self, new_state):
        if new_state > 1:
            return
        self.state = new_state
        self.textVar.set(self.btn_texts[self.state])
