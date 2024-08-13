
#include "request-data.h"
#include "plugin-support.h"

#include <pugixml.hpp>
#include <obs-module.h>

struct request_data_handler_response parse_xml(struct request_data_handler_response response,
					       const url_source_request_data *request_data)
{
	// Parse the response as XML using pugixml
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(response.body.c_str());
	if (!result) {
		obs_log(LOG_INFO, "Failed to parse XML response: %s", result.description());
		// Return an error response
		struct request_data_handler_response responseFail;
		responseFail.error_message = result.description();
		responseFail.status_code = URL_SOURCE_REQUEST_PARSING_ERROR_CODE;
		return responseFail;
	}
	std::string parsed_output = "";
	// Get the output value
	if (request_data->output_xpath != "") {
		pugi::xpath_node_set nodes = doc.select_nodes(request_data->output_xpath.c_str());
		if (nodes.size() > 0) {
			parsed_output = nodes[0].node().text().get();
		} else {
			obs_log(LOG_INFO, "Failed to get XML value");
			// Return an error response
			struct request_data_handler_response responseFail;
			responseFail.error_message = "获取XML值失败";
			responseFail.status_code = URL_SOURCE_REQUEST_PARSING_ERROR_CODE;
			return responseFail;
		}
	} else {
		// Return the whole XML object
		parsed_output = response.body;
	}
	response.body_parts_parsed.push_back(parsed_output);
	return response;
}

struct request_data_handler_response
parse_xml_by_xquery(struct request_data_handler_response response,
		    const url_source_request_data *request_data)
{
	// Parse the response as XML using pugixml
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(response.body.c_str());
	if (!result) {
		obs_log(LOG_INFO, "Failed to parse XML response: %s", result.description());
		// Return an error response
		struct request_data_handler_response responseFail;
		responseFail.error_message = result.description();
		responseFail.status_code = URL_SOURCE_REQUEST_PARSING_ERROR_CODE;
		return responseFail;
	}
	std::string parsed_output = "";
	// Get the output value
	if (request_data->output_xquery != "") {
		pugi::xpath_query query_entity(request_data->output_xquery.c_str());
		std::string s = query_entity.evaluate_string(doc);
		parsed_output = s;
	} else {
		// Return the whole XML object
		parsed_output = response.body;
	}
	response.body_parts_parsed.push_back(parsed_output);
	return response;
}
