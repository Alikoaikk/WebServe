/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 10:45:49 by akoaik            #+#    #+#             */
/*   Updated: 2026/08/26 10:47:11 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/imports.hpp"
#include "classes/serveFile_helper.hpp"

class methods
{
      
    public : 
        Response handleDelete(const Request& req, const parse::serConfig& ser);
        Response handleGet(const Request& req, const parse::serConfig& serv);
        Response handlePost(const Request& req, const parse::serConfig& serv);
        
};