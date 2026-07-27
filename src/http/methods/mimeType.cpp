/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mimeType.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 22:22:08 by akoaik            #+#    #+#             */
/*   Updated: 2026/07/14 22:22:10 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include <string>

std::string getMimeType(const std::string& path)
{
    size_t dot = path.rfind('.');
    if (dot == std::string::npos)
        return "application/octet-stream";

    std::string ext = path.substr(dot + 1);

    if (ext == "html" || ext == "htm")
        return "text/html";
    if (ext == "css")
        return "text/css";
    if (ext == "js")
        return "application/javascript";
    if (ext == "json")
        return "application/json";
    if (ext == "txt")
        return "text/plain";
    if (ext == "png")
        return "image/png";
    if (ext == "jpg" || ext == "jpeg")
        return "image/jpeg";
    if (ext == "gif")
        return "image/gif";
    if (ext == "ico")
        return "image/x-icon";
    if (ext == "svg")
        return "image/svg+xml";
    if (ext == "pdf")
        return "application/pdf";
    if (ext == "zip")
        return "application/zip";
    if (ext == "xml")
        return "application/xml";
    if (ext == "mp4")
        return "video/mp4";
    if (ext == "mp3")
        return "audio/mpeg";
    if (ext == "wav")
        return "audio/wav";
    if (ext == "php")
        return "application/x-httpd-php";
    if (ext == "py")
        return "application/x-python";

    return "application/octet-stream";
}
