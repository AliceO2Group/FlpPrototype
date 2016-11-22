# GUI Module

GUI module that allows:
1. Push values to web-based GUIs (see Publishing values)
2. Receive notification/commands from web-based GUIs by invoking previously bound callback methods (see Receiving notifications)

## Message format
It is expected that messages sent between the running program and web GUI are JSON encoded.
Each message must contain a filedname 'name' as baased on it callabacks are invoked.
There are no further constrains.  

## Publishing values
Published values have no format constrains but as mentioned in previous section it is recommended to encode them to JSON (as it is expected on web GUI side).
In this user can invoke 'publish' method by:
1. Passing std::string
2. Passing ptree object that will be automatically converted to JSON-string

```cpp
// create a blob
 std::string blob = "name=test";

// create ptree message (which will be converted to JSON)
boost::property_tree::ptree message;
message.put("name", "test");

// publish values
publisher->publish(blob);
publisher->publish(message);
```

See full example example/1-Publisher.cxx

## Receiving notifications
Received notifications/commands are passed to user specified methods.
User needs to bind a method to a message name. In order to do user calls 'bind' method that requires two parameters message name as std::string and function reference as std::function<void(std::unique_ptr<boost::property_tree::ptree>)>.
After binding callbacks user just invokes 'processRequests' method which process all requests in the buffer, and invokes propriate callbacks.

```cpp
// Example callback funtion, received message is passed as ptree (user needs to be aware of message structure)
void exampleCallback(std::unique_ptr<boost::property_tree::ptree> message) {
  std::cout << "Name: " << message->get<std::string>("name") << "; Value: " << message->get<std::string>("value") << std::endl;
}

...

// bind message name; provided function will be called whenever message with given name arrives
rep->bind(std::string("hello"), &exampleCallback);

// do something else, and call processRequests periodically to obtain new messages from queue
rep->processRequests();
```
See full example example/2-RequestNotification.cxx
