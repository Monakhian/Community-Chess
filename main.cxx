// Stuff to make httplib.h not crash
// #define CPPHTTPLIB_NO_GETIFADDRS
// #include <ifaddrs.h>

// Actual packages needed
#include "httplib.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "GamePieces.h"

int main() {
    httplib::Server svr;
    ChessBoard board;

	svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
    	std::ifstream file("index.html");

    	if (!file) {
        	res.status = 404;
        	res.set_content("index.html not found", "text/plain");
        	return;
    	}

    	std::stringstream buffer;
    	buffer << file.rdbuf();

   	 res.set_content(buffer.str(), "text/html");
	});
	
	svr.Get("/board", [&](const httplib::Request&, httplib::Response& res) {
	   	std::string json = board.to_json();

    	res.set_content(json, "application/json");
	});

    svr.Post("/move", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.body[0] != req.body [2] || req.body[1] != req.body[3]) {
        	board.move_piece(req.body[0] - '0', req.body[1] - '0', req.body[2] - '0', req.body[3] - '0');
        }

        res.set_content(board.to_json(), "application/json");
    });

    std::cout << "Server running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}