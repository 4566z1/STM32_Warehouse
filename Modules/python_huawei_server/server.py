import api
from typing import Union
from fastapi import FastAPI

app = FastAPI()

@app.get("/query/device")
def query_device():
    return api.get_device()

@app.get("/query/msg")
def query_msg():
    return api.get_message()

@app.get("/light/on")
def read_root():
    return api.send_message("{\"light\":1}")


@app.get("/light/off")
def light_off():
    return api.send_message("{\"light\":0}")


@app.get("/tempres/{tempres}")
def tempres_set(tempres: int):
    return api.send_message("{\"temp\":%d}" % (tempres))


@app.get("/humires/{humires}")
def tempres_set(humires: int):
    return api.send_message("{\"humi\":%d}" % (humires))
