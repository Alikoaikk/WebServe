/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 11:08:16 by akoaik            #+#    #+#             */
/*   Updated: 2026/08/26 14:14:16 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include "classes/serveFile_helper.hpp"


static std::string checkBasePath(const parse::locConfig& location)
{
    if (!location.uploadStore.empty())
        return location.uploadStore;
    if (!location.root.empty())
        return location.root;
    return "";
}

static std::string buildPath(const std::string& uri, const parse::locConfig& location, std::string basePath)
{
    std::string remainder = uri;
    if (uri.find(location.path) == 0)
        remainder = uri.substr(location.path.size());
    if (!basePath.empty() && basePath[basePath.size() - 1] == '/')
        basePath = basePath.substr(0, basePath.size() - 1);
    if (remainder.empty() || remainder == "/")
        return "";
    if (remainder[0] != '/')
        remainder = "/" + remainder;
    return basePath + remainder;
}

static Response uploadFile(const std::string& fullPath, const std::string& body, const std::string& uri)
{
    std::ofstream file(fullPath.c_str(), std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        Response res;
        res.setStatusCode(403);
        return res;
    }
    file.write(body.data(), body.size());
    file.close();

    Response res;
    res.setStatusCode(201);
    res.setHeader("Content-Type", "text/plain");
    res.setHeader("Location", uri);
    res.setBody("Created");
    return res;
}

Response methods::handlePost(const Request& req, const parse::serConfig& serv)
{
    const parse::locConfig *location =  findLocation(req._uri, serv);
    if (!location)
    {
        Response res ;
        res.setStatusCode(404);
        return res ;
    }

    std::string basePath = checkBasePath(*location);
    if (basePath.empty())
    {
        Response res;
        res.setStatusCode(500);
        return res;
    }

    std::string fullPath = buildPath(req._uri, *location, basePath);
    if (fullPath.empty())
    {
        Response res;
        res.setStatusCode(400);
        return res;
    }

    return uploadFile(fullPath, req._body, req._uri);
}



/*
    # Rest : 



    No problem — let's reset and go slow, plain and simple.

The goal, in one sentence:
Right now, if someone sends a POST request with a sneaky filename like /upload/../../etc/passwd, your server will try to write a file outside the uploads folder. We want to stop that.

Where does this go?
In POST.cpp, inside a function called buildPath. That function's job is: take the web address (URI) the client sent, and turn it into a real file path on disk, like /home/akoaik/.../uploads/myfile.cv.

What are we adding?
A small check: "does this URI contain the piece .. as a standalone part?" If yes, reject it.

How do you check that, step by step, in plain terms:

Think of the path as pieces separated by /. For example /../etc/passwd splits into: "", .., etc, passwd.

We want to look at each piece one at a time and ask: "is this piece exactly two dots (..)?"

To do that in code:
1. Start at the beginning of the string (position 0).
2. Look for the next / character, starting from where you are.
3. Everything between where you are and that / is one "piece."
4. Check: is that piece exactly ".."? If yes — stop, this is bad, reject it.
5. If not, move your position to just after that /, and go back to step 2.
6. If you reach the end of the string without ever finding a .. piece, it's safe.

That's it — it's just "walk through the string, chop it at each /, and check each chunk."

What do you do with the result?
If the check says "this is bad," you make buildPath return an empty string "" — because the code already has a rule: empty string means "something's wrong with this path," and it automatically turns into a 400 Bad Request response. So you don't need to add anything else — you're just plugging one more reason into a decision that already exists.

Then what?
Once you write that check, run make re to rebuild. If there's a typo or a missing return, the compiler will tell you exactly where.

Would you like to try writing that small piece-by-piece check yourself now, and paste it here so I can look at it before it goes in? Or would you like me to show you one line at a time as you type it?

*/