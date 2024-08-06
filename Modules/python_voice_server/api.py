# coding: utf-8
from huaweicloudsdkcore.auth.credentials import BasicCredentials
from huaweicloudsdkcore.auth.credentials import DerivedCredentials
from huaweicloudsdkcore.region.region import Region as coreRegion
from huaweicloudsdkcore.exceptions import exceptions
from huaweicloudsdkiotda.v5 import *

ak = "FR7GECGFLVEGAE8L3BI2"
sk = "PAh2u2BpsIhR6JSYtGM2KQdZEl7gX4x1sTJRB2Y8"
iotdaEndpoint = "b046fca4d3.st1.iotda-app.cn-north-4.myhuaweicloud.com"

def get_device():
    credentials = BasicCredentials(ak, sk).with_derived_predicate(DerivedCredentials.get_default_derived_predicate())
    client = IoTDAClient.new_builder().with_credentials(credentials).with_region(coreRegion(id="cn-north-4", endpoint=iotdaEndpoint)).build()

    try:
        request = ShowDeviceRequest()
        request.device_id = "66990c645830dc113ecd2974_air780"
        response = client.show_device(request)
        return response
    except exceptions.ClientRequestException as e:
        print(e.status_code)
        print(e.request_id)
        print(e.error_code)
        print(e.error_msg)

def get_message():
    credentials = BasicCredentials(ak, sk).with_derived_predicate(DerivedCredentials.get_default_derived_predicate())
    client = IoTDAClient.new_builder().with_credentials(credentials).with_region(coreRegion(id="cn-north-4", endpoint=iotdaEndpoint)).build()

    try:
        request = ShowDeviceShadowRequest()
        request.device_id = "66990c645830dc113ecd2974_air780"
        response = client.show_device_shadow(request)
        return response
    except exceptions.ClientRequestException as e:
        print(e.status_code)
        print(e.request_id)
        print(e.error_code)
        print(e.error_msg)

def send_message(msg):
    credentials = BasicCredentials(ak, sk).with_derived_predicate(DerivedCredentials.get_default_derived_predicate())
    client = IoTDAClient.new_builder().with_credentials(credentials).with_region(coreRegion(id="cn-north-4", endpoint=iotdaEndpoint)).build()

    try:
        request = CreateMessageRequest()
        request.device_id = "66990c645830dc113ecd2974_air780"
        request.body = DeviceMessageRequest(
            message=msg,
            payload_format="raw"
        )
        response = client.create_message(request)
        return response
    except exceptions.ClientRequestException as e:
        print(e.status_code)
        print(e.request_id)
        print(e.error_code)
        print(e.error_msg)
