/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseMultiPart.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 20:14:33 by akoaik            #+#    #+#             */
/*   Updated: 2026/09/03 21:45:07 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/POST_helper.hpp"

std::string parseMultipart(const std::string& body, const std::string& boundary, std::string& outFilename)
{
    std::string delimiter = "--" + boundary;

    size_t start = body.find(delimiter);
    if (start == std::string::npos)
        return "";

    size_t headerEnd = body.find("\r\n\r\n", start + delimiter.size());
    if (headerEnd == std::string::npos)
        return "";

    std::string headers = body.substr(start, headerEnd - start);
    size_t fn = headers.find("filename=\"");
    if (fn != std::string::npos)
    {
        fn += 10;
        size_t fnEnd = headers.find('"', fn);
        if (fnEnd != std::string::npos)
            outFilename = headers.substr(fn, fnEnd - fn);
    }

    size_t contentStart = headerEnd + 4;
    size_t contentEnd = body.find("\r\n" + delimiter, contentStart);
    if (contentEnd == std::string::npos)
        return "";

    return body.substr(contentStart, contentEnd - contentStart);
}

/*
    When someone uploads a file through a browser form, the file doesn't arrive alone. The browser wraps it in an envelope with
    dividers and labels around it. parseMultipart's only job is to open that envelope and take out the actual file content and
    the filename.


--------------------------------------------------------------------------------------------------

How the code peels it apart

  1. Build the real divider
  std::string delimiter = "--" + boundary;   // "--X"
  The separator inside the body always has two extra dashes in front of the boundary. So you search for --X.

  2. Find where the part starts
  size_t start = body.find(delimiter);
  if (start == std::string::npos) return "";   // no divider → not multipart → give up

  3. Find the blank line
  size_t headerEnd = body.find("\r\n\r\n", ...);
  Every part is labels → blank line → content. That blank line (\r\n\r\n) is the fold in the envelope. Everything before it is
  labels; everything after is the file.

  Content-Disposition: ... filename="hello.txt"    ← label
                                                   ← blank line  (headerEnd points here)
  Hi                                               ← content

  4. Read the filename out of the labels
  std::string headers = body.substr(start, headerEnd - start);
  size_t fn = headers.find("filename=\"");
  // grab the text between the quotes → outFilename = "hello.txt"

  5. Grab the content — between the blank line and the closing divider
  size_t contentStart = headerEnd + 4;                 // skip the 4 chars of "\r\n\r\n"
  size_t contentEnd = body.find("\r\n" + delimiter, contentStart);
  return body.substr(contentStart, contentEnd - contentStart);   // "Hi"

*/
