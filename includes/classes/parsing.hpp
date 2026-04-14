/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:06:11 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/14 02:08:28 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

#include "../headers/imports.hpp"


class parse
{
    private :

		std::string configFileName ;
        typedef struct locationConfig
        {
			std::string                 path ;
            std::vector<std::string>    methods ;
            std::string                 root ;
            std::string                 index ;
            int                         redirectCode ;
            bool                        autoindex ;
            std::string                 uploadStore ;
            std::string                 redirectUrl ;
            std::string                 cgiPass ;
        }	locConfig ;
        typedef struct serverConfig
        {
			int                         port ;
			unsigned int                clientMaxBodySize ;
			std::string					host ;
			std::map<int, std::string>	errorPages ;
			std::vector<locConfig>		locations ;
        }	serConfig ;

        std::vector<serConfig>  servers ;

    public :
        parse(std::string configFileName);
};

std::vector<std::string> tokenize(const std::string &filename);

#endif
